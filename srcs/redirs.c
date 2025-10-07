/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:34:29 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:18:57 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	close_both_and_fail(int *a, int *b)
{
	if (*a >= 0)
		close(*a);
	if (*b >= 0)
		close(*b);
	return (1);
}

int	open_in(char *path, int *fd)
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

int	open_out(char *path, int *fd, int append)
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

//ca gere les < et <<
int	input_redir(t_token *t, int *fd_in, int *fd_out)
{
	if (open_in(t->next->str, fd_in))
	{
		perror(t->next->str);
		return (close_both_and_fail(fd_in, fd_out));
	}
	return (0);
}

//ca gere les > et >>
//dd pour savoir si c'est un double redir ou simple redir
int	output_redir(t_token *t, int *fd_in, int *fd_out)
{
	int	dd;

	dd = (t->type == DROUT);
	if (open_out(t->next->str, fd_out, dd))
	{
		perror(t->next->str);
		return (close_both_and_fail(fd_in, fd_out));
	}
	return (0);
}

// static int	redir_for_token(t_token *t, int *fd_in, int *fd_out)
// {
// 	if (!t->next || t->next->type != FD)
// 		return (0);
// 	if (t->type == RINT)
// 		return (input_redir(t, fd_in, fd_out));
// 	else if (t->type == ROUT || t->type == DROUT)
// 		return (output_redir(t, fd_in, fd_out));
// 	return (0);
// }

// //on applique kes fd a la commande
// static void	apply_fds(t_cmd *cmd, int fd_in, int fd_out)
// {
// 	if (fd_in >= 0)
// 	{
// 		if (cmd->input != -1)
// 			close(cmd->input);
// 		cmd->input = fd_in;
// 	}
// 	if (fd_out >= 0)
// 	{
// 		if (cmd->output != -1)
// 			close(cmd->output);
// 		cmd->output = fd_out;
// 	}
// }

// int	apply_redirections(t_cmd *cmd, t_env *env)
// {
// 	t_token	*t;
// 	int		fd_in;
// 	int		fd_out;
// 	int		ret;

// 	(void)env;
// 	fd_in = -1;
// 	fd_out = -1;
// 	t = cmd->reds;
// 	while (t)
// 	{
// 		ret = redir_for_token(t, &fd_in, &fd_out);
// 		if (ret != 0)
// 			return (ret);
// 		t = t->next;
// 	}
// 	apply_fds(cmd, fd_in, fd_out);
// 	return (0);
// }

//AVANT DE DIVISER
// int	apply_redirections(t_cmd *cmd, t_env *env)
// {
// 	t_token	*t;
// 	int		fd_in;
// 	int		fd_out;

// 	(void)env;
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
// 		t = t->next;
// 	}
// 	if (fd_in >= 0)
// 	{
// 		if (cmd->input != -1)
// 			close(cmd->input);
// 		cmd->input = fd_in;
// 	}
// 	if (fd_out >= 0)
// 	{
// 		if (cmd->output != -1)
// 			close(cmd->output);
// 		cmd->output = fd_out;
// 	}
// 	return (0);
// }
