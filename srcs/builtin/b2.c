/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b2.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 11:52:23 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/01 11:55:47 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_setenv(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	if (!env || !key)
		return (1);
	current = *env;
	while (current)
	{
		if (current->key && !ft_strcmp(current->key, key))
		{
			if (current->value)
				free(current->value);
			if (value)
				new_node->value = ft_strdup(value);
			else
				new_node->value = ft_strdup("");
			return (0);
		}
		current = current->next;
	}
	new_node = malloc (sizeof(t_env));
	if (!new_node)
		return (1);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = ft_strdup("");
	if (!new_node->key || !new_node->value)
	{
		if (new_node->key)
			free(new_node->key);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
		return (1);
	}
	new_node->next = *env;
	*env = new_node;
	return (0);
}
