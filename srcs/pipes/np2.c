/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   np2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:26:09 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:26:33 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipeline(t_pipeline *pipeline)
{
	pipeline->current_pipe = -1;
	pipeline->prev_pipe = -1;
	pipeline->pipefd1[0] = -1;
	pipeline->pipefd1[1] = -1;
	pipeline->pipefd2[0] = -1;
	pipeline->pipefd2[1] = -1;
}

void	child(t_cmd *cmd_list, t_env *env)
{
	setup_signals_child();
	exec_simple_cmd(cmd_list, env, -1);
	exit(127);
}

pid_t	*pid_array(t_pipeline *pipeline, t_cmd *cmd)
{
	int		i;
	t_cmd	*tmp;
	pid_t	*pids;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	pipeline->nb_cmd = i;
	pids = malloc (sizeof(pid_t) * i);
	if (!pids)
		return (perror("malloc"), NULL);
	return (pids);
}

int	create_pipe(t_pipeline *pipeline)
{
	if (pipeline->current_pipe == 0)
	{
		if (pipeline->pipefd1[0] != -1)
		{
			close(pipeline->pipefd1[0]);
			close(pipeline->pipefd1[1]);
		}
		if (pipe(pipeline->pipefd1) == -1)
			return (perror("creation pipe 1 failed"), 1);
		fcntl(pipeline->pipefd1[0], F_SETFD, FD_CLOEXEC);
		fcntl(pipeline->pipefd1[1], F_SETFD, FD_CLOEXEC);
	}
	else
	{
		if (pipeline->pipefd2[0] != -1)
		{
			close(pipeline->pipefd2[0]);
			close(pipeline->pipefd2[1]);
		}
		if (pipe(pipeline->pipefd2) == -1)
			return (perror("creation pipe 2 failed"), 1);
		fcntl(pipeline->pipefd2[0], F_SETFD, FD_CLOEXEC);
		fcntl(pipeline->pipefd2[1], F_SETFD, FD_CLOEXEC);
	}
	return (0);
}

int	heredocs_token(t_token *token, t_cmd *cmd, t_env *env)
{
	int		hfd;
	int		expand;
	char	*d;
	int		i;

	expand = 1;
	d = token->next->str;
	if (d && d[0] == '\1')
	{
		expand = 0;
		i = 0;
		while (d[i])
		{
			d[i] = d[i + 1];
			i++;
		}
	}
	hfd = ms_heredoc(d, expand, env, cmd);
	if (hfd < 0)
		return (1);
	if (cmd->input != -1)
		close(cmd->input);
	cmd->input = hfd;
	return (0);
}
