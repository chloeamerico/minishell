/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:06:14 by camerico          #+#    #+#             */
/*   Updated: 2025/09/26 14:59:29 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//on LIT depuis le prev_pipe;
//on ECRIT dans le current_pipe;
int	child_process(int cmd_index, t_pipeline *pipeline, t_cmd *cmd, t_env *env)
{
	if (cmd_index > 0)		//on configure STDIN si on est pas a la 1 ere cmd
	{
		if (pipeline->prev_pipe == 0)
			dup2(pipeline->pipefd1[0], STDIN_FILENO);
		else
			dup2(pipeline->pipefd2[0], STDIN_FILENO);
	}
	if (cmd_index < (pipeline->nb_cmd - 1))		//on configure STDOUT si on n'est PAS a la derniere cmd
	{
		if (pipeline->current_pipe == 0)
			dup2(pipeline->pipefd1[1], STDOUT_FILENO);
		else
			dup2(pipeline->pipefd2[1], STDOUT_FILENO);
	}
	setup_signals_child();
	get_global()->child_pid = 0;
	if (apply_redirections(cmd, env))   // <<, <, >, >> (annule en cas d’erreur / Ctrl-C heredoc)
		_exit(1);
	close_all_pipes(pipeline);
	exec_simple_cmd(cmd, env);
	return(0);
}


//on ferme tous les pipes et on les met a -1
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
	char	*full_path;

	if (access(cmd, X_OK) == 0)
		return(ft_strdup(cmd));
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL); // faire attention si on retire le chemin
	paths = (ft_split(envp[i] + 5, ':'));
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		if (access(full_path, X_OK) == 0)
			return (free(tmp), free_tab(paths), full_path);
		free(full_path);
		free(tmp);
		i++;
	}
	free_tab(paths);
	return (NULL);
}

static void	cmd_not_found(char **cmd_arg, char *cmd_name)
{
	ft_putstr_fd("Command ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(" not found\n", STDERR_FILENO);
	free_tab(cmd_arg);
	exit(127);
}

int	exec_simple_cmd(t_cmd *cmd, t_env *env)
{
	char	**envp;
	char	*cmd_path;
	char	**cmd_arg;
	int		status;

	if (apply_redirections(cmd, env) < 0)
		exit(1);
	envp = env_to_array(env);
	if (!envp)
		return(1);
	// if (!cmd)
	// 	exit(1);
	cmd_arg = tokens_to_array(cmd->args);
	if (!cmd_arg)
	{
		free_tab(envp);
		exit(1);
	}
	if (!cmd_arg[0])
	{
		ft_printf("Error : invalid command");
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
	cmd_path = find_cmd_path(cmd_arg[0], envp);
	if (!cmd_path)
	{
		free_cmd_list(cmd);
		free_tab(envp);
		free_env(env);
		cmd_not_found(cmd_arg, cmd_arg[0]); 	//exit a la fin
		return(1);
	}
	// free_env(env);
	execve(cmd_path, cmd_arg, envp);
	perror("execve failed");
	free_tab(cmd_arg);
	free_tab(envp);
	free(cmd_path);
	exit(1);
}


