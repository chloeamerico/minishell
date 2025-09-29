/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:14:18 by lleichtn          #+#    #+#             */
/*   Updated: 2025/09/29 16:00:41 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"

static void sigint_interactive(int sig)
{
	(void)sig;
	get_global()->last_status = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// void	setup_signals_interactive(void)
// {
// 	struct sigaction sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	sa.sa_handler = sigint_interactive;
// 	sigaction(SIGINT, &sa, NULL);
// 	signal(SIGQUIT, SIG_IGN);
// }

void	setup_signals_interactive(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	// SIGINT
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = sigint_interactive;
	sigaction(SIGINT, &sa_int, NULL);
	
	// SIGQUIT - ne pas ignorer, juste ne rien faire dans le parent
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// static void	handler_hd(int s)
// {
// 	(void)s;
// 	get_global()->last_status = 130;
// 	get_global()->hd_interrupted = 1;
// 	_exit(130);
// }

void	setup_signals_hd(void)
{
// 	struct sigaction sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	sa.sa_handler = handler_hd;
// 	sigaction(SIGINT, &sa, NULL);
// 	signal(SIGQUIT, SIG_IGN);
signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_IGN);
}