/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:34:29 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/03 12:08:17 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"

// static int	close_both_and_fail(int *a, int *b)
// {
// 	if (*a >= 0)
// 		close(*a);
// 	if (*b >= 0)
// 		close(*b);
// 	return (1);
// }

// static int	open_in(char *path, int *fd)
// {
// 	int	f;

// 	if (*fd >= 0)
// 		close(*fd);
// 	f = open(path, O_RDONLY);
// 	if (f < 0)
// 		return (1);
// 	*fd = f;
// 	return (0);
// }

// static int	open_out(char *path, int *fd, int append)
// {
// 	int	f;
// 	int	flags;

// 	if (*fd >= 0)
// 		close(*fd);
// 	flags = O_WRONLY | O_CREAT;
// 	if (append)
// 		flags = flags | O_APPEND;
// 	else
// 		flags = flags | O_TRUNC;
// 	f = open(path, flags, 0644);
// 	if (f < 0)
// 		return (1);
// 	*fd = f;
// 	return (0);
// }

// static int	do_heredoc(t_token *lim, int *fd, t_env *env, t_cmd *cmd)
// {
// 	int		expand;
// 	char	*d;
// 	int		i;
// 	int		hfd;

// 	expand = 1;
// 	d = lim->str;
// 	if (d && d[0] == '\1')
// 	{
// 		expand = 0;
// 		i = 0;
// 		while (d[i])
// 		{
// 			d[i] = d[i + 1];
// 			i++;
// 		}
// 	}
// 	hfd = ms_heredoc(d, expand, env, cmd);
// 	if (hfd < 0)
// 		return (1);
// 	if (*fd >= 0)
// 		close(*fd);
// 	*fd = hfd;
// 	return (0);
// }

// int	apply_redirections(t_cmd *cmd, t_env *env)
// {
// 	t_token	*t;
// 	int		fd_in;
// 	int		fd_out;

// 	fd_in = -1;
// 	fd_out = -1;
// 	t = cmd->reds;
// 	while (t)
// 	{
// 		if (t->type == RINT && t->next && t->next->type == FD)
// 		{
// 			if (open_in(t->next->str, &fd_in))
// 			{
// 				perror(t->next->str);
// 				return (close_both_and_fail(&fd_in, &fd_out));
// 			}
// 		}
// 		else if (t->type == ROUT && t->next && t->next->type == FD)
// 		{
// 			if (open_out(t->next->str, &fd_out, 0))
// 			{
// 				perror(t->next->str);
// 				return (close_both_and_fail(&fd_in, &fd_out));
// 			}
// 		}
// 		else if (t->type == DROUT && t->next && t->next->type == FD)
// 		{
// 			if (open_out(t->next->str, &fd_out, 1))
// 			{
// 				perror(t->next->str);
// 				return (close_both_and_fail(&fd_in, &fd_out));
// 			}
// 		}
// 		else if (t->type == DRIN && t->next && t->next->type == LIM)
// 		{
// 			if (do_heredoc(t->next, &fd_in, env, cmd))
// 			{
// 				if (get_global()->hd_interrupted)
// 					get_global()->last_status = 130;
// 				return (close_both_and_fail(&fd_in, &fd_out));
// 			}
// 		}
// 		t = t->next;
// 	}
// 	if (fd_in >= 0)
// 	{
// 		if (dup2(fd_in, STDIN_FILENO) < 0)
// 		{
// 			perror("dup2");
// 			close(fd_in);
// 			if (fd_out >= 0)
// 				close(fd_out);
// 			return (1);
// 		}
// 		close(fd_in);
// 	}
// 	if (fd_out >= 0)
// 	{
// 		if (dup2(fd_out, STDOUT_FILENO) < 0)
// 		{
// 			perror("dup2");
// 			close(fd_out);
// 			return (1);
// 		}
// 		close(fd_out);
// 	}
// 	return (0);
// }

// #include "minishell.h"

// static int	open_infile(t_cmd *cmd, t_token *file)
// {
// 	int	fd;

// 	if (!file || !file->str)
// 	{
// 		write(2, "minishell: syntax error\n", 24);
// 		return (-1);
// 	}
// 	fd = open(file->str, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror(file->str);
// 		return (-1);
// 	}
// 	if (cmd->input != -1)
// 		close(cmd->input);
// 	cmd->input = fd;
// 	return (0);
// }

// static int	open_outfile(t_cmd *cmd, t_token *file, int append)
// {
// 	int	fd;
// 	int	flags;

// 	if (!file || !file->str)
// 	{
// 		write(2, "minishell: syntax error\n", 24);
// 		return (-1);
// 	}
// 	flags = O_CREAT | O_WRONLY;
// 	if (append)
// 		flags |= O_APPEND;
// 	else
// 		flags |= O_TRUNC;
// 	fd = open(file->str, flags, 0644);
// 	if (fd < 0)
// 	{
// 		perror(file->str);
// 		return (-1);
// 	}
// 	if (cmd->output != -1)
// 		close(cmd->output);
// 	cmd->output = fd;
// 	return (0);
// }

// static int	write_line(int wfd, char *s)
// {
// 	if (!s)
// 		return (0);
// 	if (write(wfd, s, ft_strlen(s)) < 0)
// 		return (-1);
// 	if (write(wfd, "\n", 1) < 0)
// 		return (-1);
// 	return (0);
// }

// static int	do_heredoc(t_cmd *cmd, t_token *lim, t_env *env)
// {
// 	int		p[2]; char *line;
// 	(void)env;
// 	if (!lim || !lim->str)
// 	{
// 		write(2, "minishell: syntax error\n", 24);
// 		return (-1);
// 	}
// 	if (pipe(p) == -1) { perror("pipe"); return (-1); }
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line) { close(p[0]); close(p[1]); return (-1); }
// 		if (!ft_strcmp(line, lim->str)) { free(line); break; }
// 		if (write_line(p[1], line) == -1)
// 		{ free(line); close(p[0]); close(p[1]); perror("write"); return (-1); }
// 		free(line);
// 	}
// 	close(p[1]);
// 	if (cmd->input != -1)
// 		close(cmd->input);
// 	cmd->input = p[0];
// 	return (0);
// }

// int	apply_redirections(t_cmd *cmd, t_env *env)
// {
// 	t_token	*t; int rc;

// 	t = cmd->reds;
// 	while (t)
// 	{
// 		rc = 0;
// 		if (t->type == RINT) rc = open_infile(cmd, t->next);
// 		else if (t->type == ROUT) rc = open_outfile(cmd, t->next, 0);
// 		else if (t->type == DROUT) rc = open_outfile(cmd, t->next, 1);
// 		else if (t->type == DRIN) rc = do_heredoc(cmd, t->next, env);
// 		if (rc < 0) return (-1);
// 		if (t->next) t = t->next->next;
// 		else t = t->next;
// 	}
// 	return (0);
// }

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

static int	do_heredoc(t_token *lim, int *fd, t_env *env, t_cmd *cmd)
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
	hfd = ms_heredoc(d, expand, env, cmd);
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
	if (do_heredoc(t->next, &fd_in, env, cmd))
	{
		if (get_global()->hd_interrupted)
		{
			get_global()->last_status = 130;
			close_both_and_fail(&fd_in, &fd_out);
			return (-2);  // ← RETOURNE -2 au lieu de 1
		}
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