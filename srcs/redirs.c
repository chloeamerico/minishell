/* redirs.c */
#include "minishell.h"

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

/* si lim->str commence par '\1', le limitateur était quoté -> pas d'expansion */
static int	do_heredoc(t_token *lim, int *fd, t_env *env)
{
	int	expand;
	char	*d;
	int	i;
	int	hfd;

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
	hfd = ms_heredoc(d, expand, env);
	if (hfd < 0)
		return (1);
	if (*fd >= 0)
		close(*fd);
	*fd = hfd;
	return (0);
}

/* parcourt cmd->reds, ouvre/dup2 les bons fd ; retourne 0 si OK, 1 si erreur/annulation */
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
		if (t->type == RINT && t->next && t->next->type == FD
			&& open_in(t->next->str, &fd_in))
			return (1);
		if (t->type == ROUT && t->next && t->next->type == FD
			&& open_out(t->next->str, &fd_out, 0))
			return (1);
		if (t->type == DROUT && t->next && t->next->type == FD
			&& open_out(t->next->str, &fd_out, 1))
			return (1);
		if (t->type == DRIN && t->next && t->next->type == LIM
			&& do_heredoc(t->next, &fd_in, env))
			return (1);
		t = t->next;
	}
	if (fd_in >= 0)
	{
		dup2(fd_in, 0);
		close(fd_in);
	}
	if (fd_out >= 0)
	{
		dup2(fd_out, 1);
		close(fd_out);
	}
	return (0);
}
