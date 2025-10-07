/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_com_init2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:20:37 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:24:26 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc (sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->input = -1;
	cmd->output = -1;
	cmd->reds = NULL;
	cmd->args = NULL;
	cmd->prev = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	check_if_expand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	create_node2_with_equal(t_env *node, char *envdeb, char *equal)
{
	size_t	len;

	len = equal - envdeb;
	node->key = ft_substr(envdeb, 0, len);
	if (!node->key)
		return (1);
	node->value = ft_strdup(equal + 1);
	if (!node->value)
	{
		free(node->key);
		return (1);
	}
	return (0);
}
