/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:45:09 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/01 16:46:30 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*hd_env(int op, t_env *val)
{
	static t_env	*keep = NULL;

	if (op == 0)
		keep = val;
	return (keep);
}

t_cmd	*hd_cmd(int op, t_cmd *val)
{
	static t_cmd	*keep = NULL;

	if (op == 0)
		keep = val;
	return (keep);
}
// static void	hd_sigint_handler(int sig)
// {	
// 	t_env	*e;
// 	t_cmd	*c;
// 	(void)sig;
// 	e = hd_env(1, NULL);
// 	c = hd_cmd(1, NULL);
// 	if (e)
// 		free_env(e);
// 	if (c)
// 		free_cmd_list(c);
// 	_exit(130);
// }