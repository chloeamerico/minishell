/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:51:17 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/02 15:55:58 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*hd_expand(char *s, t_env *env, int last)
{
	t_token	fake;
	t_token	one;
	char	*res;

	fake.next = &one;
	fake.prev = NULL;
	one.next = NULL;
	one.prev = &fake;
	one.type = WRD;
	one.str = ft_strdup(s);
	if (!one.str)
		return (ft_strdup(""));
	expand_tokens(&one, env, last);
	res = ft_strdup(one.str);
	free(one.str);
	return (res);
}

static int	hd_loop(int wfd, char *delim, int expand, t_env *env)
{
	char	*l;
	char	*e;

	while (1)
	{
		if (get_global()->hd_interrupted)
			return (close(wfd), 1);
		l = readline("> ");
		if (!l)
			return (close(wfd), 0);
		if (get_global()->hd_interrupted)
		{
			free(l);
			return (close(wfd), 1);
		}
		if (!ft_strcmp(l, delim))
			return (free(l), close(wfd), 0);
		if (expand)
		{
			e = hd_expand(l, env, get_global()->last_status);
			if (!e)
				return (free(l), close(wfd), 1);
			free(l);
			l = e;
		}
		if (write(wfd, l, ft_strlen(l)) < 0 || write(wfd, "\n", 1) < 0)
			return (free(l), close(wfd), 1);
		free(l);
	}
	return (0);
}

// t_env	*hd_env(int op, t_env *val)
// {
// 	static t_env	*keep = NULL;

// 	if (op == 0)
// 		keep = val;
// 	return (keep);
// }

// t_cmd	*hd_cmd(int op, t_cmd *val)
// {
// 	static t_cmd	*keep = NULL;

// 	if (op == 0)
// 		keep = val;
// 	return (keep);
// }
static void	hd_sigint_handler(int sig)
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
	_exit(130);
}

int	ms_heredoc(char *delim, int expand, t_env *env, t_cmd *cmd)
{
	int		p[2];
	pid_t	pid;
	int		st;
	void	(*old_handler)(int);

	if (pipe(p) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (close(p[0]), close(p[1]), -1);
	if (pid == 0)
	{
		hd_env(0, env);
		hd_cmd(0, cmd);
		signal(SIGINT, hd_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		close(p[0]);
		if (hd_loop(p[1], delim, expand, env))
		{
			close(p[1]);
			free_env(env);
			free_cmd_list(cmd);
			_exit(130);
		}
		close(p[1]);
		free_env(env);
		free_cmd_list(cmd);
		_exit(0);
	}
	close(p[1]);
	old_handler = signal(SIGINT, SIG_IGN);
	if (waitpid(pid, &st, 0) < 0)
	{
		signal(SIGINT, old_handler);
		return (close(p[0]), -1);
	}
	signal(SIGINT, old_handler);
	if (WIFEXITED(st) && WEXITSTATUS(st) == 0)
	{
		fcntl(p[0], F_SETFD, FD_CLOEXEC);
		return (p[0]);
	}
	if (WIFEXITED(st) && WEXITSTATUS(st) == 130)
		get_global()->hd_interrupted = 1;
	close(p[0]);
	return (-1);
}
