/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chpl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 18:55:36 by camerico          #+#    #+#             */
/*   Updated: 2025/10/06 17:38:19 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Nouvelle fonction à ajouter
static void	close_other_heredocs(t_cmd *all_cmds, t_cmd *current_cmd)
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

int	child_process(int cmd_index, t_pipeline *pipeline,
	t_cmd *cmd, t_env *env, pid_t *pids)
{
	int		rc;
	t_cmd	*first_cmd;

	free(pids);
	rc = apply_redirections(cmd, env);
	if (rc != 0)
	{
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
	first_cmd = cmd;
	while (first_cmd->prev)
		first_cmd = first_cmd->prev;
	close_other_heredocs(first_cmd, cmd);
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
	setup_signals_child();
	get_global()->child_pid = 0;
	close_all_pipes(pipeline);
	exec_simple_cmd(cmd, env, rc);
	return (0);
}

void	close_all_pipes(t_pipeline *pipeline)
{
	if (pipeline->pipefd1[0] != -1)
	{
		close(pipeline->pipefd1[0]);
		pipeline->pipefd1[0] = -1;
	}
	if (pipeline->pipefd1[1] != -1)
	{
		close(pipeline->pipefd1[1]);
		pipeline->pipefd1[1] = -1;
	}
	if (pipeline->pipefd2[0] != -1)
	{
		close(pipeline->pipefd2[0]);
		pipeline->pipefd2[0] = -1;
	}
	if (pipeline->pipefd2[1] != -1)
	{
		close(pipeline->pipefd2[1]);
		pipeline->pipefd2[1] = -1;
	}
}

static char	*find_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*tmp;
	char	*full;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full, F_OK) == 0)
			return (free_tab(paths), full);
		free(full);
		i++;
	}
	return (free_tab(paths), NULL);
}

static int	check_path(char *cmd)
{
	struct stat	st;

	if (access(cmd, F_OK) != 0)
	{
		fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);
		return (127);
	}
	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "minishell: %s: is a directory\n", cmd);
		return (126);
	}
	if (access(cmd, X_OK) != 0)
	{
		fprintf(stderr, "minishell: %s: Permission denied\n", cmd);
		return (126);
	}
	return (0);
}

static char	*find_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (envp[i] + 5);
}

//cree le path final
static char	*build_final_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

static int	check_command_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*full_path;
	int		found_but_no_exec;

	found_but_no_exec = 0;
	i = 0;
	while (paths[i])
	{
		full_path = build_final_path(paths[i], cmd);
		if (access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) != 0)
				found_but_no_exec = 1;
			else
			{
				free(full_path);
				return (0);
			}
		}
		free(full_path);
		i++;
	}
	if (found_but_no_exec)
		return (126);
	return (127);
}

static int	print_cmd_error(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	int		result;

	if (ft_strchr(cmd, '/'))
		return (check_path(cmd));
	path_env = find_path_env(envp);
	if (!path_env)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd);
		return (127);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd);
		return (127);
	}
	result = check_command_in_paths(paths, cmd);
	free_tab(paths);
	if (result == 126)
		fprintf(stderr, "minishell: %s: Permission denied\n", cmd);
	else if (result == 127)
		fprintf(stderr, "minishell: %s: command not found\n", cmd);
	return (result);
}

static int	exec_simple_cmd_part2(char **cmd_arg, char **envp)
{
	int		status;
	int		error_code;
	char	*cmd_path;

	signal(SIGPIPE, SIG_IGN);
	if (try_run_builtin(cmd_arg, &envp, &status))
	{
		free_tab(cmd_arg);
		free_tab(envp);
		exit(status);
	}
	error_code = print_cmd_error(cmd_arg[0], envp);
	if (error_code != 0)
	{
		free_tab(cmd_arg);
		free_tab(envp);
		exit(error_code);
	}
	cmd_path = find_cmd_path(cmd_arg[0], envp);
	if (!cmd_path)
	{
		free_tab(cmd_arg);
		free_tab(envp);
		exit(127);
	}
	execve(cmd_path, cmd_arg, envp);
	perror("execve failed");
	free(cmd_path);
	free_tab(cmd_arg);
	free_tab(envp);
	exit(126);
}

int	exec_simple_cmd(t_cmd *cmd, t_env *env, int rc)
{
	char	**envp;
	char	**cmd_arg;
	t_cmd	*first;

	if (rc == -1)
	{
		if (apply_redirections(cmd, env) < 0)
		{
			first = cmd;
			while (first && first->prev)
				first = first->prev;
			free_cmd_list(first);
			free_env(env);
			exit(1);
		}
	}
	envp = env_to_array(env);
	if (!envp)
	{
		first = cmd;
		while (first && first->prev)
			first = first->prev;
		free_cmd_list(first);
		free_env(env);
		exit(1);
	}
	cmd_arg = tokens_to_array(cmd->args);
	if (!cmd_arg)
	{
		free_tab(envp);
		first = cmd;
		while (first && first->prev)
			first = first->prev;
		free_cmd_list(first);
		free_env(env);
		exit(1);
	}
	if (!cmd_arg[0] || !cmd_arg[0][0])
	{
		free_tab(cmd_arg);
		free_tab(envp);
		first = cmd;
		while (first && first->prev)
			first = first->prev;
		free_cmd_list(first);
		free_env(env);
		exit(127);
	}
	first = cmd;
	while (first && first->prev)
		first = first->prev;
	free_cmd_list(first);
	free_env(env);
	return (exec_simple_cmd_part2(cmd_arg, envp));
}
