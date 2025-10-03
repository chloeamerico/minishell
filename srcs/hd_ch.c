/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_ch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 13:50:00 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/02 19:26:14 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <signal.h>

static int	hd_child_loop(int wfd, char *delim)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (!ft_strcmp(line, delim))
		{
			free(line);
			break;
		}
		if (write(wfd, line, ft_strlen(line)) < 0 || write(wfd, "\n", 1) < 0)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

static int	hd_make_fd(char *delim, int *rfd)
{
	int		p[2];
	pid_t	pid;
	int		st;

	if (pipe(p) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(p[0]);
		close(p[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(p[0]);
		if (hd_child_loop(p[1], delim) == -1)
			_exit(1);
		close(p[1]);
		_exit(0);
	}
	close(p[1]);
	if (waitpid(pid, &st, 0) == -1)
	{
		close(p[0]);
		return (-1);
	}
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
	{
		close(p[0]);
		return (-2);
	}
	if (WIFEXITED(st) && WEXITSTATUS(st) == 0)
	{
		*rfd = p[0];
		return (0);
	}
	close(p[0]);
	return (-1);
}

static int	apply_heredocs_cmd(t_cmd *cmd, t_env *env)
{
	t_token	*t;
	int		fd;
	int		rc;

	(void)env;
	t = cmd->reds;
	while (t)
	{
		if (t->type == DRIN && t->next && t->next->str)
		{
			fd = -1;
			rc = hd_make_fd(t->next->str, &fd);
			if (rc == -2)
				return (-2);
			if (rc == -1)
				return (-1);
			if (cmd->input != -1)
				close(cmd->input);
			cmd->input = fd;
		}
		t = t->next;
	}
	return (0);
}

int	prepare_heredocs(t_cmd *cmds, t_env *env)
{
	t_cmd	*c;
	int		rc;

	c = cmds;
	while (c)
	{
		rc = apply_heredocs_cmd(c, env);
		if (rc != 0)
			return (rc);
		c = c->next;
	}
	return (0);
}
