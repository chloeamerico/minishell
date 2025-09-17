/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:56:44 by camerico          #+#    #+#             */
/*   Updated: 2025/09/17 14:23:59 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"

// //trouver l’index où se trouve la var recherchée
// static int	find_env_index(char **envp, char *key)
// {
// 	int		i;
// 	size_t	len;

// 	if (!key)
// 		return (-1);
// 	len = ft_strlen(key);
// 	i = 0;
// 	while (envp[i])
// 	{
// 		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
// 			return (i);
// 		i++;
// 	}
// 	return (-1); //si pas trouvee
// }

// //supp var a implementer
// int	ft_unset(char **args, char ***envp)
// {
// 	int	i;
// 	int	idx;

// 	i = 1;
// 	while (args[i])
// 	{
// 		idx = find_env_index(*envp, args[i]);
// 		if (idx != -1)
// 		{
// 			free((*envp)[idx]);
// 			while ((*envp)[idx])
// 			{
// 				(*envp)[idx] = (*envp)[idx + 1]; // decale elem pour combler la suppression
// 				idx++;
// 			}
// 		}
// 		i++;
// 	}
// 	return (0);
// }

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
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
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
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = 1;
		}
		else
		{
			idx = find_env_index(*envp, args[i]);
			if (idx != -1)
			{
				free((*envp)[idx]);  // Libère l'ancien élément
				// Décale tous les éléments suivants
				while ((*envp)[idx + 1])
				{
					(*envp)[idx] = (*envp)[idx + 1];
					idx++;
				}
				(*envp)[idx] = NULL;  // Termine par NULL
			}
		}
		i++;
	}
	return (exit_status);
}

