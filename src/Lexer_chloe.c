/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer_chloe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:35:58 by camerico          #+#    #+#             */
/*   Updated: 2025/05/28 19:45:43 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// OJECTIF : lire la ligne de commande (input) et la decouper en une liste de tokens (t_tokens *)
// en appelant des sous-fonctions pour chaque type de mot rencontre

#include "minishell.h"

// la fonction va parcourir l'input(on saute les espaces) et si il rencontre un mot, on appelle une fonction,
// et quand on rencontre un operateur on appelle une autre fonction
t_token *lexer(char *input)
{
	int	i;
	t_token	*token;

	i = 0;
	token = NULL;
	while(input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
			i++;
		else if (input[i] == 39 || input[i] == 34)		// si c'est un "" ou ''
			recup_quote_word(input, &i, &token);						// a creer, fonction qui va recup le mot en ajoutant chaque char au mot jusqu'a que les quotes se referment
		else if (is_operator(input[i]))
			recup_operator(input, &i, &token);						// a creer, fonction qui va recup l'operator et le transformer en token
		else
			recup_word();							// a creer, fonction qui va recup le mot et en creer un token
	}												// pas de i++ ici car on avance directement a l'interieur de chaque fonciton appellees
	return(token);
}

void	recup_quote_word(char *input, int *i, t_token **token)
{
	char	c;			// "" ou ''
	int		start;
	char	*word;
	int		len;
	
	c = input[*i];
	(*i)++;				// on commence au 1er char apres les quote
	start = *i;
	
	while(input[*i] != c)
		(*i)++;
	len = *i - start;
	word = ft_substr(input, start, len);
	if (!word)
		return;
	
	create_token(token, );		//il faut creer la fonction qui va transformer le mot en token
}

int	is_operator(char c)
{
	if (c == '<' || c == '<<' || c == '>'
		|| c == '>>' || c == '|')
		return(0);
	else
		return (1);
}

void	recup_operator(char *input, int *i, t_token **token)
{
	char	*operator;
	t_token_type	type;
	
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		operator = ft_strdup(">>");
		type = APPEND;
		(*i) += 2;		//on avance direct dans cette fonction
	}

	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		operator = ft_strdup("<<");
		type = HEREDOC;
		(*i) += 2;
	}
	
	else if (input[*i] == '<')
	{
		operator = ft_strdup("<");
		type = REDIR_IN;
		(*i) += 1;
	}
	
	else if (input[*i] == '>')
	{
		operator = ft_strdup(">");
		type = REDIR_OUT;
		(*i) += 1;
	}

	else if (input[*i] == '|')
	{
		operator = ft_strdup("|");
		type = PIPE;
		(*i) += 1;
	}

	if (operator)
		return;

	create_token(token, );					//il faut creer la fonction qui va transformer l'operator en token et stoker dans le bon type
}
