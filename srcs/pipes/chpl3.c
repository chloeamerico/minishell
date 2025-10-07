/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chpl3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:41:06 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:41:14 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child_process(int cmd_index, t_pipec *pipec)
{
	int	rc;

	free(pipec->pids);
	rc = apply_redirections(pipec->current_cmd, pipec->env);
	if (rc != 0)
		cleanup_child_error(pipec->pipeline, pipec->current_cmd, pipec->env);
	get_first_and_close_hd(pipec->current_cmd);
	setup_input(pipec->current_cmd, pipec->pipeline, cmd_index);
	setup_output(pipec->current_cmd, pipec->pipeline, cmd_index);
	setup_signals_child();
	get_global()->child_pid = 0;
	close_all_pipes(pipec->pipeline);
	exec_simple_cmd(pipec->current_cmd, pipec->env, rc);
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

// static char	*find_cmd_path(char *cmd, char **envp)
// {
// 	int		i;
// 	char	**paths;
// 	char	*tmp;
// 	char	*full;

// 	if (ft_strchr(cmd, '/'))
// 		return (ft_strdup(cmd));
// 	i = 0;
// 	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
// 		i++;
// 	if (!envp[i])
// 		return (NULL);
// 	paths = ft_split(envp[i] + 5, ':');
// 	if (!paths)
// 		return (NULL);
// 	i = 0;
// 	while (paths[i])
// 	{
// 		tmp = ft_strjoin(paths[i], "/");
// 		full = ft_strjoin(tmp, cmd);
// 		free(tmp);
// 		if (access(full, F_OK) == 0)
// 			return (free_tab(paths), full);
// 		free(full);
// 		i++;
// 	}
// 	return (free_tab(paths), NULL);
// }

char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (envp[i] + 5);
}

char	*check_path_dir(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!full)
		return (NULL);
	if (access(full, F_OK) == 0)
		return (full);
	free(full);
	return (NULL);
}

char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*result;

	i = 0;
	while (paths[i])
	{
		result = check_path_dir(paths[i], cmd);
		if (result)
			return (free_tab(paths), result);
		i++;
	}
	return (free_tab(paths), NULL);
}
