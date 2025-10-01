/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:56:13 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 11:30:54 by lleichtn         ###   ########.fr       */
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
