/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:55:55 by camerico          #+#    #+#             */
/*   Updated: 2025/08/15 17:41:34 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//on libere tous les noeuds de la structure token
void	free_token(t_token *token)
{
	t_token	*tmp;

	while(token)
	{
		tmp = token;
		token = token->next;
		free(tmp->str);
		free(tmp);
	}
}

//on libere les cmd
void	free_commands(t_cmd *cmd)
{
	t_cmd	*tmp;

	while(cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		free_token(tmp->args);
		free_token(tmp->reds);
		free(tmp);
	}
}

//on libere struct env
void	free_env(t_env *env)
{
	t_env	*tmp;

	while(env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

