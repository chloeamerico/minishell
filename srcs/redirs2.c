/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:17:04 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:20:16 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_for_token(t_token *t, int *fd_in, int *fd_out)
{
	if (!t->next || t->next->type != FD)
		return (0);
	if (t->type == RINT)
		return (input_redir(t, fd_in, fd_out));
	else if (t->type == ROUT || t->type == DROUT)
		return (output_redir(t, fd_in, fd_out));
	return (0);
}

//on applique kes fd a la commande
void	apply_fds(t_cmd *cmd, int fd_in, int fd_out)
{
	if (fd_in >= 0)
	{
		if (cmd->input != -1)
			close(cmd->input);
		cmd->input = fd_in;
	}
	if (fd_out >= 0)
	{
		if (cmd->output != -1)
			close(cmd->output);
		cmd->output = fd_out;
	}
}

int	apply_redirections(t_cmd *cmd, t_env *env)
{
	t_token	*t;
	int		fd_in;
	int		fd_out;
	int		ret;

	(void)env;
	fd_in = -1;
	fd_out = -1;
	t = cmd->reds;
	while (t)
	{
		ret = redir_for_token(t, &fd_in, &fd_out);
		if (ret != 0)
			return (ret);
		t = t->next;
	}
	apply_fds(cmd, fd_in, fd_out);
	return (0);
}
