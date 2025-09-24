/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:07:54 by camerico          #+#    #+#             */
/*   Updated: 2025/09/24 15:56:14 by camerico         ###   ########.fr       */
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