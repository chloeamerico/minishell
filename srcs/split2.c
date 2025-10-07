/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:11:10 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:14:26 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//verifie les metachar
int	is_metachar(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

// met a jour le state
void	quote_state(char c, int *state)
{
	if (*state == STATE_NONE && c == '\'')
		*state = STATE_SINGLE;
	else if (*state == STATE_NONE && c == '"')
		*state = STATE_DOUBLE;
	else if (*state == STATE_SINGLE && c == '\'')
		*state = STATE_NONE;
	else if (*state == STATE_DOUBLE && c == '"')
		*state = STATE_NONE;
}

//obtient fin d'un mot en respectant les quotes
int	get_word_len(char *line, int *i)
{
	int	state;
	int	len;

	state = STATE_NONE;
	len = 0;
	while (line[*i])
	{
		quote_state(line[*i], &state);
		if (state == STATE_NONE && (line[*i] == ' '
				|| is_metachar(line[*i])))
			break ;
		(*i)++;
		len++;
	}
	return (len);
}

// Alloue mot (sans quotes) extrait de start à end
char	*get_word(char *line, int start, int end)
{
	int		copy_state;
	char	*word;
	int		j;

	copy_state = STATE_NONE;
	word = malloc (sizeof(char) * (end - start + 1));
	j = 0;
	if (!word)
		return (NULL);
	while (start < end)
		word[j++] = line[start++];
	word[j] = '\0';
	return (word);
}
