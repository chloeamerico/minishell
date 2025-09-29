/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:06:14 by camerico          #+#    #+#             */
/*   Updated: 2025/09/29 14:18:00 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"


// //on LIT depuis le prev_pipe;
// //on ECRIT dans le current_pipe;
// int	child_process(int cmd_index, t_pipeline *pipeline, t_cmd *cmd, t_env *env, pid_t *pids)
// {
// 	free(pids);
// 	if (cmd_index > 0)
// 	{
// 		if (pipeline->prev_pipe == 0)
// 			dup2(pipeline->pipefd1[0], STDIN_FILENO);
// 		else
// 			dup2(pipeline->pipefd2[0], STDIN_FILENO);
// 	}
// 	if (cmd_index < (pipeline->nb_cmd - 1))
// 	{
// 		if (pipeline->current_pipe == 0)
// 			dup2(pipeline->pipefd1[1], STDOUT_FILENO);
// 		else
// 			dup2(pipeline->pipefd2[1], STDOUT_FILENO);
// 	}
// 	setup_signals_child();
// 	get_global()->child_pid = 0;
// 	if (apply_redirections(cmd, env))
// 		_exit(1);
// 	close_all_pipes(pipeline);
// 	exec_simple_cmd(cmd, env);
// 	return(0);
// }


// //on ferme tous les pipes et on les met a -1
// void close_all_pipes(t_pipeline *pipeline)
// {
// 	if(pipeline->pipefd1[0] != -1)
// 	{
// 		close(pipeline->pipefd1[0]);
// 		pipeline->pipefd1[0] = -1;
// 	}
// 	if(pipeline->pipefd1[1] != -1)
// 	{
// 		close(pipeline->pipefd1[1]);
// 		pipeline->pipefd1[1] = -1;
// 	}
// 	if(pipeline->pipefd2[0] != -1)
// 	{
// 		close(pipeline->pipefd2[0]);
// 		pipeline->pipefd2[0] = -1;
// 	}
// 	if(pipeline->pipefd2[1] != -1)
// 	{
// 		close(pipeline->pipefd2[1]);
// 		pipeline->pipefd2[1] = -1;
// 	}
// }

// static char	*find_cmd_path(char *cmd, char **envp)
// {
// 	int		i;
// 	char	**paths;
// 	char	*tmp;
// 	char	*full_path;

// 	if (access(cmd, F_OK) == 0)
// 	{
// 		if (access(cmd, X_OK) == 0)
// 			return(ft_strdup(cmd));
// 		return (NULL);
// 	}
	
// 	i = 0;
// 	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
// 		i++;
// 	if (!envp[i])
// 		return (NULL);
	
// 	paths = (ft_split(envp[i] + 5, ':'));
// 	i = 0;
// 	while (paths[i])
// 	{
// 		tmp = ft_strjoin(paths[i], "/");
// 		full_path = ft_strjoin(tmp, cmd);
// 		free(tmp);
		
// 		if (access(full_path, F_OK) == 0)
// 		{
// 			if (access(full_path, X_OK) == 0)
// 			{
// 				free_tab(paths);
// 				return (full_path);
// 			}
// 			free(full_path);
// 			free_tab(paths);
// 			return (NULL);
// 		}
// 		free(full_path);
// 		i++;
// 	}
// 	free_tab(paths);
// 	return (NULL);
// }

// // static void	cmd_not_found(char **cmd_arg, char *cmd_name)
// // {
// // 	ft_putstr_fd("Command ", STDERR_FILENO);
// // 	ft_putstr_fd(cmd_name, STDERR_FILENO);
// // 	ft_putstr_fd(" not found\n", STDERR_FILENO);
// // 	free_tab(cmd_arg);
// // 	exit(127);
// // }

// // int	exec_simple_cmd(t_cmd *cmd, t_env *env)
// // {
// // 	char	**envp;
// // 	char	*cmd_path;
// // 	char	**cmd_arg;
// // 	int		status;

// // 	if (apply_redirections(cmd, env) < 0)
// // 		exit(1);
// // 	envp = env_to_array(env);
// // 	if (!envp)
// // 		return(1);
// // 	// if (!cmd)
// // 	// 	exit(1);
// // 	cmd_arg = tokens_to_array(cmd->args);
// // 	if (!cmd_arg)
// // 	{
// // 		free_tab(envp);
// // 		exit(1);
// // 	}
// // 	if (!cmd_arg[0])
// // 	{
// // 		ft_printf("Error : invalid command\n");
// // 		free_tab(cmd_arg);
// // 		free_tab(envp);
// // 		exit(1);
// // 	}
// // 	if (try_run_builtin(cmd_arg, &envp, &status))
// // 	{
// // 		free_tab(cmd_arg);
// // 		free_tab(envp);
// // 		free_env(env);
// // 		free_cmd_list(cmd);
// // 		exit(status);
// // 	}
// // 	cmd_path = find_cmd_path(cmd_arg[0], envp);
// // 	if (!cmd_path)
// // 	{
// // 		free_cmd_list(cmd);
// // 		free_tab(envp);
// // 		free_env(env);
// // 		cmd_not_found(cmd_arg, cmd_arg[0]); 	//exit a la fin
// // 		return(1);
// // 	}
// // 	// free_env(env);
// // 	execve(cmd_path, cmd_arg, envp);
// // 	perror("execve failed");
// // 	free_tab(cmd_arg);
// // 	free_tab(envp);
// // 	free(cmd_path);
// // 	exit(1);
// // }

// static int	check_cmd_error(char *cmd, char **envp)
// {
// 	int		i;
// 	char	**paths;
// 	char	*tmp;
// 	char	*full_path;
	
// 	// Vérifier si c'est un chemin absolu ou relatif
// 	if (ft_strchr(cmd, '/'))
// 	{
// 		if (access(cmd, F_OK) != 0)
// 		{
// 			fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);
// 			return (127);
// 		}
// 		if (access(cmd, X_OK) != 0)
// 		{
// 			fprintf(stderr, "minishell: %s: Permission denied\n", cmd);
// 			return (126);
// 		}
// 		return (0);
// 	}
	
// 	// Chercher dans le PATH
// 	i = 0;
// 	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
// 		i++;
// 	if (!envp[i])
// 	{
// 		fprintf(stderr, "minishell: %s: command not found\n", cmd);
// 		return (127);
// 	}
	
// 	paths = ft_split(envp[i] + 5, ':');
// 	i = 0;
// 	while (paths[i])
// 	{
// 		tmp = ft_strjoin(paths[i], "/");
// 		full_path = ft_strjoin(tmp, cmd);
// 		free(tmp);
		
// 		if (access(full_path, F_OK) == 0)
// 		{
// 			if (access(full_path, X_OK) != 0)
// 			{
// 				fprintf(stderr, "minishell: %s: Permission denied\n", cmd);
// 				free(full_path);
// 				free_tab(paths);
// 				return (126);
// 			}
// 			free(full_path);
// 			free_tab(paths);
// 			return (0);
// 		}
// 		free(full_path);
// 		i++;
// 	}
// 	free_tab(paths);
// 	fprintf(stderr, "minishell: %s: command not found\n", cmd);
// 	return (127);
// }

// int	exec_simple_cmd(t_cmd *cmd, t_env *env)
// {
// 	char	**envp;
// 	char	*cmd_path;
// 	char	**cmd_arg;
// 	int		status;
// 	int		error_code;

// 	if (apply_redirections(cmd, env) < 0)
// 		exit(1);
// 	envp = env_to_array(env);
// 	if (!envp)
// 		exit(1);
// 	cmd_arg = tokens_to_array(cmd->args);
// 	if (!cmd_arg)
// 	{
// 		free_tab(envp);
// 		exit(1);
// 	}
// 	if (!cmd_arg[0])
// 	{
// 		ft_printf("Error : invalid command\n");
// 		free_tab(cmd_arg);
// 		free_tab(envp);
// 		exit(1);
// 	}
// 	if (try_run_builtin(cmd_arg, &envp, &status))
// 	{
// 		free_tab(cmd_arg);
// 		free_tab(envp);
// 		free_env(env);
// 		free_cmd_list(cmd);
// 		exit(status);
// 	}
// 	error_code = check_cmd_error(cmd_arg[0], envp);
// 	if (error_code != 0)
// 	{
// 		free_tab(cmd_arg);
// 		free_tab(envp);
// 		free_env(env);
// 		free_cmd_list(cmd);
// 		exit(error_code);
// 	}
// 	cmd_path = find_cmd_path(cmd_arg[0], envp);
// 	if (!cmd_path)
// 	{
// 		free_tab(cmd_arg);
// 		free_tab(envp);
// 		free_env(env);
// 		free_cmd_list(cmd);
// 		exit(127);
// 	}
// 	execve(cmd_path, cmd_arg, envp);
// 	perror("execve failed");
// 	free_tab(cmd_arg);
// 	free_tab(envp);
// 	free(cmd_path);
// 	free_env(env);
// 	free_cmd_list(cmd);
// 	exit(1);
// }

#include "minishell.h"

int	child_process(int cmd_index, t_pipeline *pipeline, t_cmd *cmd, t_env *env, pid_t *pids)
{
	free(pids);
	if (cmd_index > 0)
	{
		if (pipeline->prev_pipe == 0)
			dup2(pipeline->pipefd1[0], STDIN_FILENO);
		else
			dup2(pipeline->pipefd2[0], STDIN_FILENO);
	}
	if (cmd_index < (pipeline->nb_cmd - 1))
	{
		if (pipeline->current_pipe == 0)
			dup2(pipeline->pipefd1[1], STDOUT_FILENO);
		else
			dup2(pipeline->pipefd2[1], STDOUT_FILENO);
	}
	setup_signals_child();
	get_global()->child_pid = 0;
	close_all_pipes(pipeline);
	exec_simple_cmd(cmd, env);
	return(0);
}

void close_all_pipes(t_pipeline *pipeline)
{
	if(pipeline->pipefd1[0] != -1)
	{
		close(pipeline->pipefd1[0]);
		pipeline->pipefd1[0] = -1;
	}
	if(pipeline->pipefd1[1] != -1)
	{
		close(pipeline->pipefd1[1]);
		pipeline->pipefd1[1] = -1;
	}
	if(pipeline->pipefd2[0] != -1)
	{
		close(pipeline->pipefd2[0]);
		pipeline->pipefd2[0] = -1;
	}
	if(pipeline->pipefd2[1] != -1)
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
	free_tab(paths);
	return (NULL);
}


static int	print_cmd_error(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*tmp;
	char	*full_path;
	int		found_but_no_exec;
	
	found_but_no_exec = 0;
	
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0)
		{
			fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);
			return (127);
		}
		if (access(cmd, X_OK) != 0)
		{
			fprintf(stderr, "minishell: %s: Permission denied\n", cmd);
			return (126);
		}
		return (0);
	}
	
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd);
		return (127);
	}
	
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd);
		return (127);
	}
	
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		
		if (access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) != 0)
				found_but_no_exec = 1;
			else
			{
				free(full_path);
				free_tab(paths);
				return (0);
			}
		}
		free(full_path);
		i++;
	}
	free_tab(paths);
	
	if (found_but_no_exec)
	{
		fprintf(stderr, "minishell: %s: Permission denied\n", cmd);
		return (126);
	}
	
	fprintf(stderr, "minishell: %s: command not found\n", cmd);
	return (127);
}

int	exec_simple_cmd(t_cmd *cmd, t_env *env)
{
	char	**envp;
	char	*cmd_path;
	char	**cmd_arg;
	int		status;
	int		error_code;

	if (apply_redirections(cmd, env) < 0)
		exit(1);
	
	envp = env_to_array(env);
	if (!envp)
		exit(1);
	cmd_arg = tokens_to_array(cmd->args);
	if (!cmd_arg)
	{
		free_tab(envp);
		exit(1);
	}
	if (!cmd_arg[0])
	{
		ft_printf("Error : invalid command\n");
		free_tab(cmd_arg);
		free_tab(envp);
		exit(1);
	}
	
	if (try_run_builtin(cmd_arg, &envp, &status))
	{
		free_tab(cmd_arg);
		free_tab(envp);
		free_env(env);
		free_cmd_list(cmd);
		exit(status);
	}
	
	error_code = print_cmd_error(cmd_arg[0], envp);
	if (error_code != 0)
	{
		free_tab(cmd_arg);
		free_tab(envp);
		free_env(env);
		free_cmd_list(cmd);
		exit(error_code);
	}
	
	cmd_path = find_cmd_path(cmd_arg[0], envp);
	if (!cmd_path)
	{
		free_tab(cmd_arg);
		free_tab(envp);
		free_env(env);
		free_cmd_list(cmd);
		exit(127);
	}
	
	execve(cmd_path, cmd_arg, envp);
	
	perror("execve failed");
	free_tab(cmd_arg);
	free_tab(envp);
	free(cmd_path);
	free_env(env);
	free_cmd_list(cmd);
	exit(126);
}