/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:29:26 by camerico          #+#    #+#             */
/*   Updated: 2025/10/02 18:48:20 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_pipeline(t_pipeline *pipeline)
{
	pipeline->current_pipe = -1;
	pipeline->prev_pipe = -1;
	pipeline->pipefd1[0] = -1;
	pipeline->pipefd1[1] = -1;
	pipeline->pipefd2[0] = -1;
	pipeline->pipefd2[1] = -1;
}

static void	child(t_cmd *cmd_list, t_env *env)
{
	setup_signals_child();
	exec_simple_cmd(cmd_list, env);
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

static int	create_pipe(t_pipeline *pipeline)
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
	pids = pid_array(&pipeline, cmd_list);
	if (!pids)
		return (1);
	init_pipeline(&pipeline);
	pipec.current_cmd = cmd_list;
	pipec.pipeline = &pipeline;
	pipec.pids = pids;
	pipec.env = env;
	if (loop_pipe(&pipec, 0))
		return (free(pids), 1);
	exit_status = wait_children_pid(&pipeline, pids);
	free(pids);
	return (exit_status);
}

int	one_cmd_without_pipe(t_cmd *cmd_list, t_env *env)
{
	pid_t	pid;
	int		status;
	int		sig;

	pid = fork();
	if (pid == 0)
		child(cmd_list, env);
	else if (pid > 0)
	{
		if (waitpid(pid, &status, 0) == -1)
			return (1);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write(2, "Quit (core dumped)\n", 20);
			else if (sig == SIGINT)
				write(2, "\n", 1);
			return (128 + sig);
		}
		return (1);
	}
	return (perror("fork"), 1);
}

static int	loop_pipe2(t_pipec *pipec, int cmd_index)
{
	pipec->pipeline->current_pipe = cmd_index % 2;
	if (cmd_index != 0)
		pipec->pipeline->prev_pipe = (cmd_index - 1) % 2;
	else
		pipec->pipeline->prev_pipe = -1;
	if (pipec->current_cmd->next)
		create_pipe(pipec->pipeline);
	pipec->pids[cmd_index] = fork();
	if (pipec->pids[cmd_index] == -1)
	{
		perror("error : fork");
		close_all_pipes(pipec->pipeline);
		while (cmd_index > 0)
		{
			cmd_index--;
			waitpid(pipec->pids[cmd_index], NULL, 0);
		}
		return (1);
	}
	else if (pipec->pids[cmd_index] == 0)
		child_process(cmd_index, pipec->pipeline, pipec->current_cmd, pipec->env, pipec->pids);
	else
		parent_process(pipec->pipeline, cmd_index);
	return (0);
}

int	loop_pipe(t_pipec *pipec, int cmd_index)
{
	while (pipec->current_cmd)
	{
		if (loop_pipe2(pipec, cmd_index))
			return (1);
		pipec->current_cmd = pipec->current_cmd->next;
		cmd_index++;
	}
	return (0);
}
