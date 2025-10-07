/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:07:54 by camerico          #+#    #+#             */
/*   Updated: 2025/10/07 19:51:30 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	has_non_empty_value(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	if (s[i] != '=')
		return (0);
	if (s[i + 1] == '\0')
		return (0);
	return (1);
}

int	print_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (has_non_empty_value(envp[i]))
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
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
