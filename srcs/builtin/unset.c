/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:56:44 by camerico          #+#    #+#             */
/*   Updated: 2025/10/09 11:18:44 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_env_index(char **envp, char *key)
{
	int		i;
	size_t	len;

	if (!key || !envp)
		return (-1);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0
			&& (envp[i][len] == '=' || envp[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

int	ft_unset(char **args, char ***envp)
{
	int	i;
	int	idx;
	int	exit_status;

	if (!args || !envp || !*envp)
		return (0);
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		idx = find_env_index(*envp, args[i]);
		if (idx != -1)
		{
			free((*envp)[idx]);
			while ((*envp)[idx + 1])
			{
				(*envp)[idx] = (*envp)[idx + 1];
				idx++;
			}
			(*envp)[idx] = NULL;
		}
		i++;
	}
	return (exit_status);
}

// //AVANT DE REDUIRE
// int	ft_unset(char **args, char ***envp)
// {
// 	int	i;
// 	int	idx;
// 	int	exit_status;

// 	if (!args || !envp || !*envp)
// 		return (0);
// 	exit_status = 0;
// 	i = 1;
// 	while (args[i])
// 	{
// 		idx = find_env_index(*envp, args[i]);
// 		if (idx != -1)
// 		{
// 			free((*envp)[idx]);
// 			while ((*envp)[idx + 1])
// 			{
// 				(*envp)[idx] = (*envp)[idx + 1];
// 				idx++;
// 			}
// 			(*envp)[idx] = NULL;
// 		}
// 	}
// 		i++;
// 	return (exit_status);
// }
