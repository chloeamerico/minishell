/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:40:47 by camerico          #+#    #+#             */
/*   Updated: 2025/09/25 18:39:53 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	size_env(t_env *env)
{
	int	i;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while(tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return(i);
}

//on passe la variable d'env d'une liste chainee à un double tab pour que ca puisse etre utilisé dans execve
char **env_to_array(t_env *env)
{
	int	i;
	int	size;
	t_env	*tmp;
	char	**tab;

	i = 0;
	tmp = env;
	size = size_env(env);
	tab = malloc(sizeof(char *) * (size + 1));
	if(!tab)
		return(NULL);
	while(tmp)
	{
		tab[i] = ft_strjoin(tmp->key, "=");
		tab[i] = ft_strjoin_free(tab[i], tmp->value);
		tmp = tmp->next;
		i++;
	}
	tab[i] = NULL;
	return(tab);
}

static int	size_token(t_token *token)
{
	int	i;
	t_token	*tmp;

	i = 0;
	tmp = token;
	while(tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return(i);
}

//passe la liste chainee t_token args presente dans cmd en tab 
char **tokens_to_array(t_token *args)
{
	int	i;
	int	size;
	t_token	*tmp;
	char	**tab;

	i = 0;
	tmp = args;
	size = size_token(args);
	tab = malloc(sizeof(char *) * (size + 1));
	if(!tab)
		return(NULL);
	while(tmp)
	{
		tab[i] = ft_strdup(tmp->str);
		if(!tab[i])
		{
			while(--i >= 0)
				free(tab[i]);
			free(tab);
			return(NULL);
		}
		tmp = tmp->next;
		i++;
	}
	tab[i] = NULL;
	return(tab);
}



