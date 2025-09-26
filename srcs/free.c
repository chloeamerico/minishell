/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:55:55 by camerico          #+#    #+#             */
/*   Updated: 2025/09/26 15:46:45 by camerico         ###   ########.fr       */
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

/* libère la liste de commandes :
   - ferme les fd valides
   - libère args (t_token*) et reds (t_token*)
   - libère le maillon t_cmd */
void	free_cmd_list(t_cmd *cmds)
{
	t_cmd	*n;

	while (cmds)
	{
		n = cmds->next;
		if (cmds->input >= 0)
			close(cmds->input);
		if (cmds->output >= 0)
			close(cmds->output);
		if (cmds->args)
			free_token(cmds->args);
		if (cmds->reds)
			free_token(cmds->reds);
		free(cmds);
		cmds = n;
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