/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:17:16 by camerico          #+#    #+#             */
/*   Updated: 2025/10/03 12:06:40 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_env(char **env)
{
	int		i;
	int		j;
	int		len_env;
	char	*tmp;

	len_env = 0;
	while (env[len_env])
		len_env++;
	i = 0;
	while (i < len_env - 1)
	{
		j = 0;
		while (j < len_env - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	*find_key(char *arg)
{
	int		i;
	int		j;
	char	*key;

	i = 0;
	j = 0;
	if (!arg)
		return (NULL);
	while (arg[i] != '=' && arg[i])
		i++;
	key = malloc (sizeof(char) * (i + 1));
	if (!key)
		return (NULL);
	while (j < i)
	{
		key[j] = arg[j];
		j++;
	}
	key[j] = '\0';
	return (key);
}

int	find_index(char *key, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, ft_strlen(key))
			&& (env[i][ft_strlen(key)] == '='
			|| env[i][ft_strlen(key)] == '\0' ))
			return (i);
		i++;
	}
	return (-1);
}

static int	print_export(char **env)
{
	int	i;
	int	j;

	i = 0;
	while (env && env[i])
	{
		write(1, "declare -x ", 11);
		j = 0;
		while (env[i][j] && env[i][j] != '=')
		{
			write(1, &env[i][j], 1);
			j++;
		}
		if (env[i][j] == '=' && env[i][j + 1])
		{
			write(1, "=\"", 2);
			j++;
			while (env[i][j])
			{
				write(1, &env[i][j], 1);
				j++;
			}
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

int	ft_export(char **args, char ***env)
{
	int	i;

	if (!args || !args[0])
		return (1);
	if (!args[1] && (!ft_strcmp(args[0], "export")))
	{
		sort_env(*env);
		print_export(*env);
	}	
	else
	{
		i = 1;
		while (args[i])
		{
			export_one_arg(args[i], env);
			i++;
		}
	}
	return (0);
}
