/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:56:32 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 16:31:15 by camerico         ###   ########.fr       */
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

// int	ft_exit(char **args)
// {
// 	int	code;
// 	int	fd_tty;

// 	fd_tty = open("/dev/tty", O_RDWR);
// 	if (fd_tty)
// 	{
// 		write(fd_tty, "exit\n", 5);
// 		close(fd_tty);
// 	}
// 	if (!args[1])
// 	get_global()->want_exit = 1;
// 	get_global()->exit_code = 0;
// 	return (0);

// 	if (!is_number(args[1]))
// 	{
// write(2, "exit: numeric argument required\n", 32);
// get_global()->want_exit = 1;
// get_global()->exit_code = 2;
// return (2);
// 	}
// 	if (args[2])
// 	{
// write(2, "exit: numeric argument required\n", 32);
// get_global()->want_exit = 1;
// get_global()->exit_code = 2;
// return (2);
// 	}
// code = ft_atoi(args[1]);
// get_global()->want_exit = 1;
// get_global()->exit_code = (unsigned char)code;
// return ((unsigned char)code);
// }

// SANS TTY
// int	ft_exit(char **args)
// {
// 	int	code;
// 	// int	fd_tty;		 //a remettre quand on remet le tty

// 	write(1, "exit\n", 5);
// 	if (!args[1])
// 		exit(0);
// 	if (!is_number(args[1]))
// 	{
// 		write(2, "exit: numeric argument required\n", 32);
// 		exit(2);
// 	}
// 	if (args[2])
// 	{
// 		write(2, "exit: too many arguments\n", 26);
// 		return (1);
// 	}
// 	code = ft_atoi(args[1]);
// 	exit((unsigned char)code);
// }

int	ft_exit(char **args)
{
	int	code;
	int	fd_tty;

	fd_tty = open("/dev/tty", O_RDWR);
	if (fd_tty)
	{
		write(fd_tty, "exit\n", 5);
		close(fd_tty);
		return (0);
	}
	if (!args[1])
	{
		get_global()->want_exit = 1;
		get_global()->exit_code = 0;
		return (0);
	}
	if (!is_number(args[1]))
	{
		write(2, "exit: numeric argument required\n", 32);
		get_global()->want_exit = 1;
		get_global()->exit_code = 2;
		return (2);
	}
	if (args[2])
	{
		write(2, "exit: too many arguments\n", 26);
		return (1);
	}
	code = ft_atoi(args[1]);
	get_global()->want_exit = 1;
	get_global()->exit_code = (unsigned char)code;
	return ((unsigned char)code);
}
