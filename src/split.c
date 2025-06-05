#include "minishell.h"

//verifie les metachar
static int is_metachar(char c)
{
	return(c == '|' || c == '>' || c == '<');
}

// met a jour le state
static void quote_state(char c, int *state)
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
	int state;
	int len;

	state = STATE_NONE;
	len = 0;
	while (line[*i])
	{
		quote_state(line[*i], &state);
		if (state == STATE_NONE && (line[*i] == ' ' || is_metachar(line[*i]))) //si spc ou metac stop
			break;
		(*i)++;
		len++;
	}
	return (len);
}

// Alloue mot (sans quotes) extrait de start à end
static char	*get_word(char *line, int start, int end)
{
	int copy_state;//etat de la quote
	char *word; //mot fiinal
	int j;

	copy_state = STATE_NONE;
	word = malloc(end - start + 1);
	j = 0;
	if (!word)
		return (NULL);
	while (start < end)
	{
		quote_state(line[start], &copy_state);
		if (copy_state == STATE_NONE && (line[start] == '\'' || line[start] == '"')) // si on a une quote on saute hors quote
			start++;
		else
			word[j++] = line[start++]; // on copie le carac ds le mot
	}
	word[j] = '\0';
	return (word);
}
