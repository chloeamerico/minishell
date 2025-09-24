/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:23:47 by camerico          #+#    #+#             */
/*   Updated: 2025/09/24 13:38:45 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char	*check_quotes_after_token(char *str)
// {
// 	int	i = 0;
// 	int	quotes = STATE_NONE;
// 	char	tmp[2];
// 	char	*str_without_quotes = ft_strdup("");

// 	while(str[i])
// 	{
// 		quote_state(str[i], &quotes);
// 		if(quotes == STATE_NONE && (str[i] == '\'' || str[i] == '"'))
// 			i++;
// 		else if((quotes == STATE_SINGLE && str[i] == '\'')
// 			|| (quotes == STATE_DOUBLE && str[i] == '"'))
// 			i++;
// 		else
// 		{
// 			tmp[0] = str[i];
// 			tmp[1] = '\0';
// 			i++;
// 		}
// 		str_without_quotes = ft_strjoin_free(str_without_quotes, tmp);
// 	}
// 	free(tmp);
// 	return (str_without_quotes);
// }

// void	delete_quotes(t_token *tokens)
// {
// 	t_token	*tmp;
// 	char	*new;
	
// 	tmp = tokens;
// 	while(tmp)
// 	{
// 		new = check_quotes_after_token(tmp->str);
// 		free(tmp->str);
// 		tmp->str = new;
// 		tmp = tmp->next;
// 	}
// }

static void	move_left(char *str, int i)
{
	while(str[i])
	{
		str[i] = str[i + 1];
		i++;
	}
}

static char	*check_quotes_after_token(char *str)
{
	int	i = 0;
	int	quotes = STATE_NONE;
	
	while(str[i])
	{
		quote_state(str[i], &quotes);
		if ((quotes == STATE_NONE && (str[i] == '\'' || str[i] == '"'))
			|| (quotes == STATE_SINGLE && str[i] == '\'')
			|| (quotes == STATE_DOUBLE && str[i] == '"'))
		{
			move_left(str, i);
			continue;				// permet de ne pas faire le i++ mais reprendre la boucle au while
		}
		i++;
	}
	str[i] = '\0';
	return(str);
}

void	delete_quotes(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while(tmp)
	{
		tmp->str = check_quotes_after_token(tmp->str);
		tmp = tmp->next;
	}
}
