/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:19:44 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 12:29:38 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*create_node(char *envdeb)
{
	t_env	*node;
	char	*equal;
	size_t	len;

	node = malloc (sizeof(t_env));
	if (!node)
		return (NULL);
	equal = ft_strchr(envdeb, '=');
	if (!equal)
	{
		node->key = ft_strdup(envdeb);
		if (!node->key)
			return (free(node), NULL);
		node->value = NULL;
	}
	else
	{
		len = equal - envdeb;
		node->key = ft_substr(envdeb, 0, len);
		if (!node->key)
			return (free(node), NULL);
		node->value = ft_strdup(equal + 1);
		if (!node->value)
			return (free(node->key), free(node), NULL);
	}
	node->next = NULL;
	return (node);
}

t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new;

	head = NULL;
	current = NULL;
	while (*envp)
	{
		new = create_node(*envp);
		if (!new)
			return (free_env(head), NULL);
		if (!head)
			head = new;
		else
			current->next = new;
		current = new;
		envp++;
	}
	return (head);
}

t_env	*get_env_list(t_env *new_env)
{
	static t_env	*env = NULL;

	if (new_env)
		env = new_env;
	return (env);
}

static char	*build_prompt(void)
{
	char	*cwd;
	int		need;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_strdup("minishell$ "));
	need = snprintf(NULL, 0, "%s$ ", cwd);
	if (need < 0)
	{
		free(cwd);
		return (ft_strdup("minishell$ "));
	}
	*prompt = malloc ((size_t)need + 1);
	if (!prompt)
	{
		free(cwd);
		return (ft_strdup("minishell$ "));
	}
	snprintf(prompt, (size_t)need + 1, "%s$ ", cwd);
	free(cwd);
	return (prompt);
}

int	read_line(char **line)
{
	char	*prompt;

	*prompt = build_prompt();
	*line = readline(prompt);
	free(prompt);
	if (!*line)
		return (0);
	if (**line)
		add_history(*line);
	return (1);
}
