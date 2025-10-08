/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chpl5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:42:57 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/08 10:39:22 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_command_in_paths(char **paths, char *cmd)
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

int	print_cmd_error(char *cmd, char **envp)
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

void	cleanup_exec_and_exit(char **cmd_arg, char **envp,
	char *cmd_path, int code)
{
	if (cmd_path)
		free(cmd_path);
	if (cmd_arg)
		free_tab(cmd_arg);
	if (envp)
		free_tab(envp);
	exit(code);
}

int	exec_simple_cmd_part2(char **cmd_arg, char **envp)
{
	int		status;
	int		error_code;
	char	*cmd_path;

	signal(SIGPIPE, SIG_IGN);
	if (try_run_builtin(cmd_arg, &envp, &status))
		cleanup_exec_and_exit(cmd_arg, envp, NULL, status);
	error_code = print_cmd_error(cmd_arg[0], envp);
	if (error_code != 0)
		cleanup_exec_and_exit(cmd_arg, envp, NULL, error_code);
	cmd_path = find_cmd_path(cmd_arg[0], envp);
	if (!cmd_path)
		cleanup_exec_and_exit(cmd_arg, envp, NULL, 127);
	execve(cmd_path, cmd_arg, envp);
	perror("execve failed");
	cleanup_exec_and_exit(cmd_arg, envp, cmd_path, 126);
	return (0);
}

t_cmd	*get_first_cmd(t_cmd *cmd)
{
	t_cmd	*first;

	first = cmd;
	while (first && first->prev)
		first = first->prev;
	return (first);
}
