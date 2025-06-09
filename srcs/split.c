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

//fonction qui va prendre un mot et l'ajouter au tableau array
char	**word_to_array(char **array, char *word)
{
	int	i;
	char **new_array;
	
	i = 0;
	while(array && array[i])
		i++;
	new_array = malloc(sizeof(char *) * (i + 2));	// on ajoute 2 pour garder une place pour le NULL
	if (!new_array)
		return (NULL);
	i = 0;
	while(array && array[i])
	{
		new_array[i] = array[i];	// on duplique array dans new_array
		i++;
	}
	new_array[i] = word;
	new_array[i + 1] = NULL;
	free(array);
	return (new_array);
}

int	split_input(char ***array, char *line, int i)
{
	int	start;		// pour stocker ou commence le mot/token qu'on va extraire
	char *word;		// pour stocker le mot extrait

	while(line[i])
	{
		while(line[i] == ' ')		// on saute les epace entre les mots
			i++;
		if (!line[i])
			return (1);
		start = i;
		if (is_metachar(line[i]))
		{
			if (line[i] == line[i + 1])		//on gere les << et >>
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
			return (1);
	}
	return (0);
}

char	**split_minishell(char *line)
{
	char	**array;	//c'est le tableau qui va accueillir les mots separes

	array = NULL;
	if (!line)
		return (NULL);
	if (split_input(&array, line, 0))		//fonction qui va appliquer le split
		return (NULL);
	return (array);
}