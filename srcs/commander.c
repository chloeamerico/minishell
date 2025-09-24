/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commander.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cette bg d chloe <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:43:32 by lleichtn          #+#    #+#             */
/*   Updated: 2025/07/16 14:09:09 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

// represente les commandes entre deux pipes
static t_cmd *create_cmd(void)
{
t_cmd*cmd;

cmd = malloc(sizeof(t_cmd));
if (!cmd)
return (NULL);
cmd->input = -1; //pas init
cmd->output = -1; //pas init
cmd->reds = NULL;
cmd->args = NULL;
cmd->prev = NULL; //cmd av si pipe
cmd->next = NULL; //cmd suiv si pipe
return (cmd);
}

// cree la liste args ou reds de t_cmd
static int add_token(t_token **dest, t_token *src)
{
	t_token*copy;
	t_token*tmp;

	copy = malloc(sizeof(t_token));
	if (!copy)
		return (1);
	copy->str = ft_strdup(src->str);
	if(!copy->str)
		return(free(copy), 1);
	copy->type = src->type; // on garde le même type cmd wrd...
	copy->next = NULL;
	copy->prev = NULL;
	if (!*dest)
		*dest = copy;
	else
	{
		tmp = *dest;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = copy;
		copy->prev = tmp;
	}
	return (0);
}

// Trie les tokens dans la bonne liste de la commande
static int assign_token(t_cmd *cmd, t_token *tkn)
{
	if (tkn->type == CMD || tkn->type == WRD)
		return (add_token(&cmd->args, tkn)); // range ds arg
	if (tkn->type == RINT || tkn->type == ROUT
		|| tkn->type == DRIN || tkn->type == DROUT
			|| tkn->type == FD || tkn->type == LIM)
		return (add_token(&cmd->reds, tkn)); // range ds reds
	return (0);
}

// Sépare les commandes à chaque PIPE et distribue les tokens dans args/reds
static int fill_cmds(t_cmd **head, t_token *tkn)
{
	t_cmd *curr;

	curr = create_cmd(); //prem cmd
	if (!curr)
		return (1);
	*head = curr;
	while (tkn)
	{
		if (tkn->type == PIPE) // nv pipe = nv cmd
		{
			curr->next = create_cmd(); // nv cmd
			if (!curr->next)
				return (1);
			curr->next->prev = curr;
			curr = curr->next;
		}
		else if (assign_token(curr, tkn)) // tri
			return (1);
		tkn = tkn->next;
	}
	return (0);
}

// Transforme la liste de tokens en liste de commandes prêtes à exécuter
t_cmd *parse_commands(t_token *tokens)
{
	t_cmd*cmds;

	cmds = NULL;
	if (!tokens)
		return (NULL);
	if (fill_cmds(&cmds, tokens))
		return (NULL);
	return (cmds); // retourne la cmd
}
