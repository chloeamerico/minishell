/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:34:29 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/01 13:13:57 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	close_both_and_fail(int *a, int *b)
{
	if (*a >= 0)
		close(*a);
	if (*b >= 0)
		close(*b);
	return (1);
}

static int	open_in(char *path, int *fd)
{
	int	f;

	if (*fd >= 0)
		close(*fd);
	f = open(path, O_RDONLY);
	if (f < 0)
		return (1);
	*fd = f;
	return (0);
}

static int	open_out(char *path, int *fd, int append)
{
	int	f;
	int	flags;

	if (*fd >= 0)
		close(*fd);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;
	f = open(path, flags, 0644);
	if (f < 0)
		return (1);
	*fd = f;
	return (0);
}

static int	do_heredoc(t_token *lim, int *fd, t_env *env)
{
	int		expand;
	char	*d;
	int		i;
	int		hfd;

	expand = 1;
	d = lim->str;
	if (d && d[0] == '\1')
	{
		expand = 0;
		i = 0;
		while (d[i])
		{
			d[i] = d[i + 1];
			i++;
		}
	}
	hfd = ms_heredoc(d, expand, env, lim);
	if (hfd < 0)
		return (1);
	if (*fd >= 0)
		close(*fd);
	*fd = hfd;
	return (0);
}

int	apply_redirections(t_cmd *cmd, t_env *env)
{
	t_token	*t;
	int		fd_in;
	int		fd_out;

	fd_in = -1;
	fd_out = -1;
	t = cmd->reds;
	while (t)
	{
		if (t->type == RINT && t->next && t->next->type == FD)
		{
			if (open_in(t->next->str, &fd_in))
			{
				perror(t->next->str);
				return (close_both_and_fail(&fd_in, &fd_out));
			}
		}
		else if (t->type == ROUT && t->next && t->next->type == FD)
		{
			if (open_out(t->next->str, &fd_out, 0))
			{
				perror(t->next->str);
				return (close_both_and_fail(&fd_in, &fd_out));
			}
		}
		else if (t->type == DROUT && t->next && t->next->type == FD)
		{
			if (open_out(t->next->str, &fd_out, 1))
			{
				perror(t->next->str);
				return (close_both_and_fail(&fd_in, &fd_out));
			}
		}
		else if (t->type == DRIN && t->next && t->next->type == LIM)
		{
			if (do_heredoc(t->next, &fd_in, env))
			{
				if (get_global()->hd_interrupted)
					get_global()->last_status = 130;
				return (close_both_and_fail(&fd_in, &fd_out));
			}
		}
		t = t->next;
	}
	if (fd_in >= 0)
	{
		if (dup2(fd_in, STDIN_FILENO) < 0)
		{
			perror("dup2");
			close(fd_in);
			if (fd_out >= 0)
				close(fd_out);
			return (1);
		}
		close(fd_in);
	}
	if (fd_out >= 0)
	{
		if (dup2(fd_out, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			close(fd_out);
			return (1);
		}
		close(fd_out);
	}
	return (0);
}
