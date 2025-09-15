/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:56:13 by camerico          #+#    #+#             */
/*   Updated: 2025/09/15 11:56:16 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char **args, char ***envp)
{
	(void)envp;
	if (!args[1])
	{
		if (chdir(getenv("HOME")) != 0)
		{
			write(2, "cd: HOME not set or inaccessible\n", 33);
			return (1);
		}
		return (0);
	}
	if (chdir(args[1]) != 0)
	{
		write(2, "cd: ", 4);
		perror(args[1]);
		return (1);
	}
	return (0);
}
