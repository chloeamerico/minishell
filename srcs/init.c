/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:19:44 by camerico          #+#    #+#             */
/*   Updated: 2025/09/17 14:38:53 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	**init_env(char **envp)
// {
// 	int		i;
// 	char	**copy;

// 	i = 0;
// 	while (envp[i])
// 		i++;
// 	copy = malloc(sizeof(char *) * (i + 1));
// 	if (!copy)
// 		return (NULL);
// 	i = 0;
// 	while (envp[i])
// 	{
// 		copy[i] = strdup(envp[i]);
// 		if (!copy[i])
// 			return (NULL);
// 		i++;
// 	}
// 	copy[i] = NULL;
// 	return (copy);
// }

// char	**get_env(char **env)
// {
// 	static char	**env_storage = NULL; // la sigleton donc

// 	if (env)
// 		env_storage = env;
// 	return (env_storage);
// }

// Signal envoyer lors de ctrl +C
// static void	sigint_handler(int signum)
// {
// 	(void)signum;
// 	write(1, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// void	setup_signals(void)
// {
// 	signal(SIGINT, sigint_handler);
// 	signal(SIGQUIT, SIG_IGN);
// }

static t_env	*create_node(char *envdeb)
{
	t_env	*node;
	char	*equal;
	size_t	len;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	equal = ft_strchr(envdeb, '=');
	if (!equal)
	{
		node->key = ft_strdup(envdeb);
		node->value = NULL;
	}
	else
	{
		len = equal - envdeb;
		node->key = ft_substr(envdeb, 0, len);
		node->value = ft_strdup(equal + 1);
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
			return (NULL);
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

static char *build_prompt(void)
{
    char *cwd = getcwd(NULL, 0);                // alloc par libc
    if (!cwd)
        return ft_strdup("minishell$ ");        // fallback si erreur

    // taille nécessaire pour "<cwd>$ " + '\0'
    int need = snprintf(NULL, 0, "%s$ ", cwd);
    if (need < 0) {
        free(cwd);
        return ft_strdup("minishell$ ");
    }
    char *prompt = malloc((size_t)need + 1);
    if (!prompt) {
        free(cwd);
        return ft_strdup("minishell$ ");
    }
    snprintf(prompt, (size_t)need + 1, "%s$ ", cwd);
    free(cwd);
    return prompt;
}

// int	read_line(char **line)
// {
// 	*line = readline("minishell$ "); //met minishell$ sur le bash
// 	if (!*line)
// 		return (0);
// 	if (**line)
// 		add_history(*line); //ajout la ligne de commande
// 	return (1);
// }

int read_line(char **line)
{
    char *prompt = build_prompt();         // <- prompt dynamique
    *line = readline(prompt);
    free(prompt);

    if (!*line)
        return (0);
    if (**line)
        add_history(*line);
    return (1);
}