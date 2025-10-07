/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chpl2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:38:46 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:39:43 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_other_heredocs(t_cmd *all_cmds, t_cmd *current_cmd)
{
	t_cmd	*cmd;

	cmd = all_cmds;
	while (cmd)
	{
		if (cmd != current_cmd && cmd->input != -1)
		{
			close(cmd->input);
		}
		cmd = cmd->next;
	}
}

void	cleanup_child_error(t_pipeline *pipeline, t_cmd *cmd, t_env *env)
{
	t_cmd	*first_cmd;

	close_all_pipes(pipeline);
	first_cmd = cmd;
	while (first_cmd->prev)
		first_cmd = first_cmd->prev;
	free_cmd_list(first_cmd);
	free_env(env);
	if (get_global()->hd_interrupted)
		exit(130);
	exit(1);
}

void	setup_input(t_cmd *cmd, t_pipeline *pipeline, int cmd_index)
{
	if (cmd->input != -1)
	{
		dup2(cmd->input, STDIN_FILENO);
		close(cmd->input);
	}
	else if (cmd_index > 0)
	{
		if (pipeline->prev_pipe == 0)
			dup2(pipeline->pipefd1[0], STDIN_FILENO);
		else
			dup2(pipeline->pipefd2[0], STDIN_FILENO);
	}
}

void	setup_output(t_cmd *cmd, t_pipeline *pipeline, int cmd_index)
{
	if (cmd->output != -1)
	{
		dup2(cmd->output, STDOUT_FILENO);
		close(cmd->output);
	}
	else if (cmd_index < (pipeline->nb_cmd - 1))
	{
		if (pipeline->current_pipe == 0)
			dup2(pipeline->pipefd1[1], STDOUT_FILENO);
		else
			dup2(pipeline->pipefd2[1], STDOUT_FILENO);
	}
}

t_cmd	*get_first_and_close_hd(t_cmd *cmd)
{
	t_cmd	*first_cmd;

	first_cmd = cmd;
	while (first_cmd->prev)
		first_cmd = first_cmd->prev;
	close_other_heredocs(first_cmd, cmd);
	return (first_cmd);
}
