/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:40:47 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 12:11:53 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	size_env(t_env *env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**env_to_array(t_env *env)
{
	int		i;
	int		size;
	t_env	*tmp;
	char	**tab;

	i = 0;
	tmp = env;
	size = size_env(env);
	tab = malloc (sizeof(char *) * (size + 1));
	if (!tab)
		return (NULL);
	while (tmp)
	{
		tab[i] = ft_strjoin(tmp->key, "=");
		tab[i] = ft_strjoin_free(tab[i], tmp->value);
		tmp = tmp->next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

static int	size_token(t_token *token)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = token;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**tokens_to_array(t_token *args)
{
	int		i;
	int		size;
	t_token	*tmp;
	char	**tab;

	i = 0;
	tmp = args;
	size = size_token(args);
	tab = malloc (sizeof(char *) * (size + 1));
	if (!tab)
		return (NULL);
	while (tmp)
	{
		tab[i] = ft_strdup(tmp->str);
		if (!tab[i])
		{
			while (--i >= 0)
				free(tab[i]);
			free(tab);
			return (NULL);
		}
		tmp = tmp->next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
