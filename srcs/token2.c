/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:30:16 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:31:02 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(t_type type)
{
	return (type == RINT || type == ROUT || type == DRIN || type == DROUT);
}

// déduire le type du token
t_type	get_token_type(char *str)
{
	if (!str)
		return (WRD);
	if (str[0] == '|' && str[1] == '\0')
		return (PIPE);
	if (str[0] == '>' && str[1] == '\0')
		return (ROUT);
	if (str[0] == '>' && str[1] == '>' && str[2] == '\0')
		return (DROUT);
	if (str[0] == '<' && str[1] == '\0')
		return (RINT);
	if (str[0] == '<' && str[1] == '<' && str[2] == '\0')
		return (DRIN);
	return (WRD);
}

// alloue un nv token
// duplique la string
// deduit le type
//peut changer le type
t_token	*create_token(char *str, t_type prev_type)
{
	t_token	*new;

	new = malloc (sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = ft_strdup(str);
	if (!new->str)
		return (free(new), NULL);
	new->next = NULL;
	new->prev = NULL;
	new->type = get_token_type(str);
	if ((prev_type == RINT || prev_type == ROUT
			|| prev_type == DRIN || prev_type == DROUT)
		&& new->type == WRD)
	{
		if (prev_type == DRIN)
			new->type = LIM;
		else
			new->type = FD;
	}
	else if ((prev_type == PIPE)
		&& new->type == WRD)
			new->type = CMD;
	return (new);
}

int	new_token(t_token *new, t_type prev_type)
{
	if (prev_type == DRIN && new->type == LIM)
	{
		if (mark_limiter_if_quoted(new))
			return (1);
	}
	return (0);
}

void	token_to_list(t_token **head, t_token **last, t_token *new)
{
	if (*last)
	{
		(*last)->next = new;
		new->prev = *last;
	}
	else
		*head = new;
	*last = new;
}
