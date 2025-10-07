/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:02:39 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 16:03:02 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hd_sigquit_handler(int sig)
{	
	t_env	*e;
	t_cmd	*c;

	(void)sig;
	e = hd_env(1, NULL);
	c = hd_cmd(1, NULL);
	if (e)
		free_env(e);
	if (c)
		free_cmd_list(c);
	_exit(131);
}

void	hd_child_exit_error(int *p, t_env *env, t_cmd *cmd)
{
	close(p[1]);
	free_env(env);
	free_cmd_list(cmd);
	_exit(130);
}

void	hd_child_exit_success(int *p, t_env *env, t_cmd *cmd)
{
	close(p[1]);
	free_env(env);
	free_cmd_list(cmd);
	_exit(0);
}

void	close_all_exect_one(int fd_keep)
{
	struct rlimit	lim;
	int				max_fd;
	int				fd;

	if (getrlimit(RLIMIT_NOFILE, &lim) == 0)
		max_fd = (int)lim.rlim_cur;
	else
		max_fd = 1024;
	fd = 3;
	while (fd < max_fd)
	{
		if (fd != fd_keep)
			close(fd);
		fd++;
	}
}
