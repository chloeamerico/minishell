/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:56:44 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 11:34:33 by lleichtn         ###   ########.fr       */
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
			&& envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
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
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n",
				STDERR_FILENO);
			exit_status = 1;
		}
		else
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
		}
		i++;
	}
	return (exit_status);
}
