/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:23:47 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 12:49:27 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	move_left(char *str, int i)
{
	while (str[i])
	{
		str[i] = str[i + 1];
		i++;
	}
}

static char	*check_quotes_after_token(char *str)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = STATE_NONE;
	while (str[i])
	{
		quote_state(str[i], &quotes);
		if ((quotes == STATE_NONE && (str[i] == '\'' || str[i] == '"'))
			|| (quotes == STATE_SINGLE && str[i] == '\'')
			|| (quotes == STATE_DOUBLE && str[i] == '"'))
		{
			move_left(str, i);
			continue ;
		}
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	delete_quotes(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		tmp->str = check_quotes_after_token(tmp->str);
		tmp = tmp->next;
	}
}
