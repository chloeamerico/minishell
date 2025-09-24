/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:56:32 by camerico          #+#    #+#             */
/*   Updated: 2025/09/24 16:38:24 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

//works 
static int	is_number(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args)
{
	int	code;

	write(1, "exit\n", 5);
	if (!args[1])
	get_global()->want_exit = 1;
	get_global()->exit_code = 0;
	return (0);

	if (!is_number(args[1]))
	{
write(2, "exit: numeric argument required\n", 32);
get_global()->want_exit = 1;
get_global()->exit_code = 2;
return (2);
	}
	if (args[2])
	{
write(2, "exit: numeric argument required\n", 32);
get_global()->want_exit = 1;
get_global()->exit_code = 2;
return (2);
	}
code = ft_atoi(args[1]);
get_global()->want_exit = 1;
get_global()->exit_code = (unsigned char)code;
return ((unsigned char)code);
}