/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:48:07 by camerico          #+#    #+#             */
/*   Updated: 2025/09/29 17:48:47 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void init_pipeline(t_pipeline *pipeline)
{
	pipeline->current_pipe = -1;
	pipeline->prev_pipe = -1;
	pipeline->pipefd1[0] = pipeline->pipefd1[1] = -1;
	pipeline->pipefd2[0] = pipeline->pipefd2[1] = -1;
}

static void count_cmd(t_pipeline *pipeline, t_cmd *cmd)
{
	int i;
	t_cmd *tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	pipeline->nb_cmd = i;
}

pid_t *pid_array(t_pipeline *pipeline, t_cmd *cmd)
{
	int cmd_count;
	pid_t *pids;

	count_cmd(pipeline, cmd);
	cmd_count = pipeline->nb_cmd;
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (perror("malloc"), NULL);
	return (pids);
}

static int create_pipe(t_pipeline *pipeline)
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
	}
	return (0);
}

int exec_pipeline(t_cmd *cmd_list, t_env *env)
{
	t_pipeline pipeline;
	t_cmd *current_cmd;
	pid_t *pids;
	int cmd_index = 0;
	int exit_status = 0;

	if (!cmd_list)
		return (1);

	if (!cmd_list->next)
		return (one_cmd_without_pipe(cmd_list, env));
	
	pids = pid_array(&pipeline, cmd_list);
	if (!pids)
		return (1);
	init_pipeline(&pipeline);

	current_cmd = cmd_list;

	if (loop_pipe(&pipeline, cmd_index, current_cmd, pids, env))
		return (free(pids), 1);

	exit_status = wait_children_pid(&pipeline, pids);
	free(pids);
	return (exit_status);
}

int	one_cmd_without_pipe(t_cmd *cmd_list, t_env *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		exec_simple_cmd(cmd_list, env);
		exit(127);
	}
	else if (pid > 0)
	{
		if (waitpid(pid, &status, 0) == -1)
			return (1);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			int sig;

			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write(2, "Quit (core dumped)\n", 20);
			else if (sig == SIGINT)
				write(2, "\n", 1);
			return (128 + sig);
		}
		return (1);
	}
	perror("fork");
	return (1);
}

int loop_pipe(t_pipeline *pipeline, int cmd_index, t_cmd *current_cmd, pid_t *pids, t_env *env)
{
	while (current_cmd)
	{
		pipeline->current_pipe = cmd_index % 2;
		if (cmd_index != 0)
			pipeline->prev_pipe = (cmd_index - 1) % 2;
		else
			pipeline->prev_pipe = -1;

		if (current_cmd->next)
			create_pipe(pipeline);

		pids[cmd_index] = fork();

		if (pids[cmd_index] == -1)
		{
			perror("error : fork");
			close_all_pipes(pipeline);
			while (cmd_index > 0)
			{
				cmd_index--;
				waitpid(pids[cmd_index], NULL, 0);
			}
			return (1);
		}
		else if (pids[cmd_index] == 0)
		{
			child_process(cmd_index, pipeline, current_cmd, env, pids);
		}
		else
		{
			if (!current_cmd->next)
				tcsetpgrp(STDIN_FILENO, pids[cmd_index]);
			
			parent_process(pipeline, cmd_index);
		}
		current_cmd = current_cmd->next;
		cmd_index++;
	}
	return (0);
}