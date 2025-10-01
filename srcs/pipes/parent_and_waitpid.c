/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_and_waitpid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:49:40 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 12:09:33 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_process(t_pipeline *pipeline, int cmd_index)
{
	if (cmd_index > 0)
	{
		if (pipeline->prev_pipe == 0)
		{
			if (pipeline->pipefd1[0] != -1)
				close(pipeline->pipefd1[0]);
			pipeline->pipefd1[0] = -1;
		}
		else
		{
			if (pipeline->pipefd2[0] != -1)
				close(pipeline->pipefd2[0]);
			pipeline->pipefd2[0] = -1;
		}
	}
	if (cmd_index < (pipeline->nb_cmd - 1))
	{
		if (pipeline->current_pipe == 0)
		{
			if (pipeline->pipefd1[1] != -1)
				close(pipeline->pipefd1[1]);
			pipeline->pipefd1[1] = -1;
		}
		else
		{
			if (pipeline->pipefd2[1] != -1)
				close(pipeline->pipefd2[1]);
			pipeline->pipefd2[1] = -1;
		}
	}
}

int	wait_children_pid(t_pipeline *pipeline, pid_t *pid)
{
	int		i;
	int		exit_status;
	int		last_exit_status;
	void	(*old_sigquit)(int);
	int		sig;

	i = 0;
	last_exit_status = 0;
	old_sigquit = signal(SIGQUIT, SIG_DFL);
	while (i < pipeline->nb_cmd)
	{
		if (waitpid(pid[i], &exit_status, 0) == -1)
			perror("waitpid");
		else
		{
			if (i == pipeline->nb_cmd - 1)
			{
				if (WIFEXITED(exit_status))
					last_exit_status = WEXITSTATUS(exit_status);
				else if (WIFSIGNALED(exit_status))
				{
					sig = WTERMSIG(exit_status);
					last_exit_status = 128 + sig;
					if (sig == SIGQUIT)
						write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				}
			}
		}
		i++;
	}
	signal(SIGQUIT, old_sigquit);
	get_global()->last_status = last_exit_status;
	get_global()->child_pid = 0;
	return (last_exit_status);
}
