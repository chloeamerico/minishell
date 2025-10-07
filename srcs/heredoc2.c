/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:59:59 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 17:23:39 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*hd_expand(char *s, t_env *env, int last)
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

int	hd_loop_check_delim(char *l, char *delim, int wfd)
{
	(void)wfd;
	if (!ft_strcmp(l, delim))
	{
		free(l);
		return (1);
	}
	return (0);
}

// int	hd_loop_expand_and_write(int wfd, char *l, int expand, t_env *env)
// {
// 	char	*x;
// 	if (expand)
// 	{
// 		x = hd_expand(l, env, get_global()->last_status);
// 		free(l);
// 		if (!x)
// 		{ close(wfd); return (1); }
// 		if (write(wfd, x, ft_strlen(x)) == -1)
// 		{ free(x); close(wfd); return (1); }
// 		free(x);
// 	}
// 	else
// 	{
// 		if (write(wfd, l, ft_strlen(l)) == -1)
// 		{ free(l); close(wfd); return (1); }
// 		free(l);
// 	}
// 	if (write(wfd, "\n", 1) == -1)
// 	{ close(wfd); return (1); }
// 	return (0);
// }

int	hd_loop_expand_and_write(int wfd,
	char *l, int expand, t_env *env)
{
	char	*e;

	if (expand)
	{
		e = hd_expand(l, env, get_global()->last_status);
		if (!e)
		{
			free(l);
			close(wfd);
			return (1);
		}
		free(l);
		l = e;
	}
	if (write(wfd, l, ft_strlen(l)) < 0 || write(wfd, "\n", 1) < 0)
	{
		free(l);
		close(wfd);
		return (1);
	}
	free(l);
	return (0);
}

int	hd_loop(int wfd, char *delim, int expand, t_env *env)
{
	char	*l;

	while (1)
	{
		l = readline("> ");
		if (!l)
		{
			close(wfd);
			return (0);
		}
		if (get_global()->hd_interrupted)
		{
			free(l);
			close(wfd);
			return (1);
		}
		if (hd_loop_check_delim(l, delim, wfd))
		{
			close(wfd);
			return (0);
		}
		if (hd_loop_expand_and_write(wfd, l, expand, env))
			return (1);
	}
	return (0);
}

void	hd_sigint_handler(int sig)
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
	if (get_global()->hd_wfd >= 0)
	{
		close(get_global()->hd_wfd);
		get_global()->hd_wfd = -1;
	}
	_exit(130);
}
