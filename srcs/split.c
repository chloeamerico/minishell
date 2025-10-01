/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:20:04 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 12:42:53 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//verifie les metachar
static int	is_metachar(char c)
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
static int	get_word_len(char *line, int *i)
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
static char	*get_word(char *line, int start, int end)
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

//fonction qui va prendre un mot et l'ajouter au tableau array
char	**word_to_array(char **array, char *word)
{
	int		i;
	char	**new_array;

	i = 0;
	while (array && array[i])
		i++;
	new_array = malloc (sizeof(char *) * (i + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	while (array && array[i])
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = word;
	new_array[i + 1] = NULL;
	free(array);
	return (new_array);
}

int	check_close_quotes(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 39)
		{
			i++;
			while (line[i] && line[i] != 39)
				i++;
			if (!line[i])
			{
				ft_putendl_fd("Syntax error: unclosed quote", 2);
				return (1);
			}
			else
				i++;
		}
		else if (line[i] == 34)
		{
			i++;
			while (line[i] && line[i] != 34)
				i++;
			if (!line[i])
			{
				ft_putendl_fd("Syntax error: unclosed quote", 2);
				return (1);
			}
			else
				i++;
		}
		else
			i++;
	}
	return (0);
}

int	split_input(char ***array, char *line, int i)
{
	int		start;
	char	*word;	

	while (line[i])
	{
		while (line[i] == ' ')
			i++;
		if (!line[i])
			break ;
		start = i;
		if (is_metachar(line[i]))
		{
			if (line[i] == line[i + 1])
				i++;
			i++;
		}
		else
			get_word_len(line, &i);
		word = get_word(line, start, i);
		if (!word)
			return (1);
		*array = word_to_array(*array, word);
		if (!*array)
			return (free(word), 1);
	}
	return (0);
}

char	**split_minishell(char *line)
{
	char	**array;

	array = NULL;
	if (!line)
		return (NULL);
	if (check_close_quotes(line))
		return (NULL);
	if (split_input(&array, line, 0))
		return (NULL);
	return (array);
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	**split_line(char *line)
{
	return (split_minishell(line));
}
