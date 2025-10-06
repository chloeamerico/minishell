/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:55:55 by camerico          #+#    #+#             */
/*   Updated: 2025/10/06 18:05:50 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//on libere tous les noeuds de la structure token
void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->str);
		free(tmp);
	}
}

//on libere struct env
void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		free_token(tmp->args);
		free_token(tmp->reds);
		if (tmp->input != -1)
			close(tmp->input);
		if (tmp->output != -1)
			close(tmp->output);
		free(tmp);
	}
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

// void	free_pid(pid_t *pid, t_pipeline *pipeline)
// {
// 	int	i;

// 	i = 0;
// 	while (pid[i] < pipeline->nb_cmd)
// 	{
// 		free(pid[i]);
// 		i++;
// 	}
// }