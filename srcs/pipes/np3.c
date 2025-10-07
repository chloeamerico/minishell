/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   np3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:28:05 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:28:22 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	collect_all_hd_loop(t_cmd *cmd, t_env *env)
{
	t_token	*token;

	token = cmd->reds;
	while (token)
	{
		if (token->type == DRIN && token->next && token->next->type == LIM)
		{
			if (heredocs_token(token, cmd, env))
				return (1);
		}
		token = token->next;
	}
	return (0);
}

int	collect_all_heredocs(t_cmd *cmd_list, t_env *env)
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		if (collect_all_hd_loop(cmd, env))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

int	exec_pipeline(t_cmd *cmd_list, t_env *env)
{
	t_pipeline	pipeline;
	pid_t		*pids;
	int			exit_status;
	t_pipec		pipec;

	if (!cmd_list)
		return (1);
	if (!cmd_list->next)
		return (one_cmd_without_pipe(cmd_list, env));
	if (collect_all_heredocs(cmd_list, env))
		return (1);
	pids = pid_array(&pipeline, cmd_list);
	if (!pids)
		return (1);
	init_pipeline(&pipeline);
	pipec.current_cmd = cmd_list;
	pipec.first_cmd = cmd_list;
	pipec.pipeline = &pipeline;
	pipec.pids = pids;
	pipec.env = env;
	if (loop_pipe(&pipec, 0))
		return (free(pids), 1);
	exit_status = wait_children_pid(&pipeline, pids);
	free(pids);
	return (exit_status);
}
