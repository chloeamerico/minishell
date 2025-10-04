/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_and_waitpid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:49:40 by camerico          #+#    #+#             */
/*   Updated: 2025/10/03 19:56:29 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//AVANT DE REDUIRE
// void	parent_process(t_pipeline *pipeline, int cmd_index)
// {
// 	if (cmd_index > 0)
// 	{
// 		if (pipeline->prev_pipe == 0)
// 		{
// 			if (pipeline->pipefd1[0] != -1)
// 				close(pipeline->pipefd1[0]);
// 			pipeline->pipefd1[0] = -1;
// 		}
// 		else
// 		{
// 			if (pipeline->pipefd2[0] != -1)
// 				close(pipeline->pipefd2[0]);
// 			pipeline->pipefd2[0] = -1;
// 		}
// 	}
// 	if (cmd_index < (pipeline->nb_cmd - 1))
// 	{
// 		if (pipeline->current_pipe == 0)
// 		{
// 			if (pipeline->pipefd1[1] != -1)
// 				close(pipeline->pipefd1[1]);
// 			pipeline->pipefd1[1] = -1;
// 		}
// 		else
// 		{
// 			if (pipeline->pipefd2[1] != -1)
// 				close(pipeline->pipefd2[1]);
// 			pipeline->pipefd2[1] = -1;
// 		}
// 	}
// }

static void	close_pipe_fd(t_pipeline *pipeline, int pipe_chose, int index)
{
	int	*fd;

	if (pipe_chose == 0)
		fd = &pipeline->pipefd1[index];
	else
		fd = &pipeline->pipefd2[index];
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}

void	parent_process(t_pipeline *pipeline, int cmd_index)
{
	if (cmd_index > 0)
		close_pipe_fd(pipeline, pipeline->prev_pipe, 0);
	if (cmd_index < (pipeline->nb_cmd - 1))
		close_pipe_fd(pipeline, pipeline->current_pipe, 1);
	// if (cmd_index == (pipeline->nb_cmd - 1))
	// 	close_all_pipes(pipeline);
}


static int	last_children_status(int exit_status)
{
	int	sig;

	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	if (WIFSIGNALED(exit_status))
	{
		sig = WTERMSIG(exit_status);
		if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		return (128 + sig);
	}
	return (0);
}

int	wait_children_pid(t_pipeline *pipeline, pid_t *pid)
{
	int		i;
	int		exit_status;
	int		last_exit_status;
	void	(*old_sigquit)(int);

	i = 0;
	last_exit_status = 0;
	old_sigquit = signal(SIGQUIT, SIG_DFL);
	while (i < pipeline->nb_cmd)
	{
		if (waitpid(pid[i], &exit_status, 0) == -1)
			perror("waitpid");
		else if (i == pipeline->nb_cmd - 1)
			last_exit_status = last_children_status(exit_status);
		i++;
	}
	signal(SIGQUIT, old_sigquit);
	get_global()->last_status = last_exit_status;
	get_global()->child_pid = 0;
	return (last_exit_status);
}

//AVANT DE REDUIRE
// int	wait_children_pid(t_pipeline *pipeline, pid_t *pid)
// {
// 	int		i;
// 	int		exit_status;
// 	int		last_exit_status;
// 	void	(*old_sigquit)(int);
// 	int		sig;

// 	i = 0;
// 	last_exit_status = 0;
// 	old_sigquit = signal(SIGQUIT, SIG_DFL);
// 	while (i < pipeline->nb_cmd)
// 	{
// 		if (waitpid(pid[i], &exit_status, 0) == -1)
// 			perror("waitpid");
// 		else
// 		{
// 			if (i == pipeline->nb_cmd - 1)
// 			{
// 				if (WIFEXITED(exit_status))
// 					last_exit_status = WEXITSTATUS(exit_status);
// 				else if (WIFSIGNALED(exit_status))
// 				{
// 					sig = WTERMSIG(exit_status);
// 					last_exit_status = 128 + sig;
// 					if (sig == SIGQUIT)
// 						write(STDERR_FILENO, "Quit (core dumped)\n", 19);
// 				}
// 			}
// 		}
// 		i++;
// 	}
// 	signal(SIGQUIT, old_sigquit);
// 	get_global()->last_status = last_exit_status;
// 	get_global()->child_pid = 0;
// 	return (last_exit_status);
// }
