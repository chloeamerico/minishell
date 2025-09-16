/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:14:18 by lleichtn          #+#    #+#             */
/*   Updated: 2025/09/15 12:23:57 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"

//gerer sigint crtlc 
static void	ctrlc(int s)
{
	(void)s;
	get_global()->sig = SIGINT;
	get_global()->last_status = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	if (get_global()->child_pid == 0)
		rl_redisplay();
}

// gere sigint ds heredoc
static void	handler_hd(int s)
{
	(void)s;
	get_global()->last_status = 130;
	get_global()->hd_interrupted = 1; 
	write(1, "\n", 1);
	// close(0);
}

void	setup_signals_interactive(void)
{
	struct sigaction sa;

	rl_catch_signals = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = ctrlc;
	sigaction(SIGINT, &sa, 0);
	signal(SIGQUIT, SIG_IGN);
}

//restaure comportement par default ds child
void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_signals_hd(void)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler_hd;
	sigaction(SIGINT, &sa, 0);
	signal(SIGQUIT, SIG_IGN);
}
