#include "minishell.h"

void	create_token(t_token **lst, char *value, t_token_type type)
{
	t_token *new;

	new = malloc(sizeof(t_token)); //new token
	if (!new)
		return;
	new->value = ft_strdup(value);
	new->type = type; //verifie le type
	new->next = NULL;
	if (!*lst)
		*lst = new;
	else
	{
		t_token *tmp = *lst;
		while (tmp->next) // Go au dernier token
			tmp = tmp->next;
		tmp->next = new; // on le rajoute a la fin
	}
}

void	recup_word(char *input, int *i, t_token **token)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && input[*i] != ' ' && !is_operator(input[*i])
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++; //On lit jusqu'à un espace, un opérateur ou une quote
	word = ft_substr(input, start, *i - start);
	if (!word)
		return;
	create_token(token, word, WORD); // creer token WORD
	free(word);
}
