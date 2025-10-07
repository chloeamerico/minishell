/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chpl4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:41:31 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:41:57 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_cmd_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}

int	check_path(char *cmd)
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

char	*find_path_env(char **envp)
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
char	*build_final_path(char *dir, char *cmd)
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
