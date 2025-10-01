/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:51:07 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 16:25:05 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//fonction qui va creer une nouvelle variable dans l'env
static char	**add_new_line_in_env(char *arg, char **env)
{
	int		i;
	int		count;
	char	**new_env;

	count = 0;
	while (env[count])
		count++;
	new_env = malloc (sizeof(char *) * (count + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[count] = ft_strdup(arg);
	if (!new_env[count])
	{
		free(new_env);
		return (NULL);
	}
	new_env[count + 1] = NULL;
	free(env);
	return (new_env);
}

//fonction qui va modifier une variable dans l'env
//Trouver l'index de la variable à modifier
//puis Libérer l'ancienne chaîne (free(env[index]))
//Créer la nouvelle chaîne ("USER=jane")
static int	update_var_in_env(char *arg, char **env, int index)
{
	char	*new_line_env;

	new_line_env = malloc (sizeof(char) * (ft_strlen(arg) + 1));
	if (!new_line_env)
		return (1);
	ft_strcpy(new_line_env, arg);
	if (env[index])
		free(env[index]);
	env[index] = new_line_env;
	return (0);
}

void	export_one_arg(char *arg, char ***env)
{
	char	*key;
	int		index;

	key = find_key(arg);
	if (!key)
		return ;
	index = find_index(key, *env);
	if (index != -1)
	{
		if (!ft_strchr(arg, '='))
		{
			free(key);
			return ;
		}
		update_var_in_env(arg, *env, index);
		free(key);
		return ;
	}
	if (!ft_strchr(arg, '='))
	{
		if (export_one_arg2 (key, env))
		{
			free(key);
			return ;
		}
	}
	*env = add_new_line_in_env(arg, *env);
	free(key);
}

int	export_one_arg2(char *key, char ***env)
{
	int		len;
	char	*tmp;

	len = ft_strlen(key);
	tmp = malloc (sizeof(char) * (len + 2));
	if (!tmp)
		return (1) ;
	ft_strcpy(tmp, key);
	tmp[len] = '=';
	tmp[len + 1] = '\0';
	*env = add_new_line_in_env(tmp, *env);
	free(tmp);
	return (0);
}
