/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:10:31 by camerico          #+#    #+#             */
/*   Updated: 2025/09/25 15:07:09 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_redir(t_type type)
{
    return (type == RINT || type == ROUT || type == DRIN || type == DROUT);
}

static int is_metachar(t_type type)
{
    return (is_redir(type) || type == PIPE);
}

// déduire le type du token
static t_type get_token_type(char *str)
{
    if (!str)
        return (WRD); //si null on renvoie wrd
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
static t_token	*create_token(char *str, t_type prev_type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = ft_strdup(str); // copie str split ds tkn
	if(!new->str)
		return(free(new), NULL);
	new->next = NULL;
	new->prev = NULL;
	new->type = get_token_type(str); // recup le type du tkn
	if ((prev_type == RINT || prev_type == ROUT
		|| prev_type == DRIN || prev_type == DROUT)
    	&& new->type == WRD)
	{
   		if (prev_type == DRIN)
        	new->type = LIM; // cas pour trouver lim
    	else
        	new->type = FD; //cas pr trouver le fd
	}
	else if ((prev_type == PIPE) 
		&& new->type == WRD)
    	new->type = CMD;
	return (new);
}

//fonction qui cree une liste chainee de t_token
//gere les liens entre next et prev
// return la tete de liste (head)
// t_token	*tokenize(char **split)
// {
// 	t_token	*head; //fst tkn celui return
// 	t_token	*last; //lst tkn
// 	t_token	*new; //tkn que l on cree
// 	t_type	prev_type; //type du tkn precedent

// 	head = NULL;
// 	last = NULL;
// 	prev_type = PIPE; //comme ca le premier mot est un cmd
// 	while (*split)
// 	{
// 		new = create_token(*split, prev_type);
// 		if (!new)
// 			return (NULL);
// 		if (last)		// donc si au moins 1 token a deja ete cree
// 		{
// 			last->next = new;
// 			new->prev = last;
// 		}
// 		else
// 			head = new;
// 		last = new;
// 		prev_type = new->type;
// 		split++;
// 	}
// 	return
t_token	*tokenize(char **split)
{
	t_token	*head;
	t_token	*last;
	t_token	*new;
	t_type	prev_type;

	head = NULL;
	last = NULL;
	prev_type = PIPE;
	while (*split)
	{
		new = create_token(*split, prev_type);
		if (!new)
			return (free_token(head), NULL);
		if (prev_type == DRIN && new->type == LIM)
		{
			if (mark_limiter_if_quoted(new))
			{
				free(new->str);
				free(new);
				return (NULL);
			}
		}
		if (last)
		{
			last->next = new;
			new->prev = last;
		}
		else
			head = new;
		last = new;
		prev_type = new->type;
		split++;
	}
	return (head);
}

//est ce que la liste de token generee est correcte / on a le droit de l'executee 
// si return (0) (= invalide)
int	validate_tokens(t_token *tkn)
{
    if (!tkn)
        return (0);
    if (tkn->type == PIPE) //si le premier token est un pipe => invalid
        return (0);
    while (tkn)
    {
        if (tkn->type == PIPE)
        {
            if (!tkn->prev || !tkn->next || tkn->next->type == PIPE)	// gere le : |... ou ... | ou ... ||...
                return (0);
        }
        if (is_redir(tkn->type))
        {
            if (!tkn->next)			// gere le ... > (ou n'imp quelle redir)
                return (0);
            if (is_metachar(tkn->next->type))		// gere si il y a une redir vers metachar
                return (0);
        }
        tkn = tkn->next;
    }
    return (1);
}
int	mark_limiter_if_quoted(t_token *tok)
{
	char	*s;
	int		i;
	int		hasq;
	int		len;
	char	*tmp;

	s = tok->str;
	i = 0;
	hasq = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			hasq = 1;
		i++;
	}
	if (!hasq)
		return (0);
	len = ft_strlen(s);
	tmp = malloc(len + 2);
	if (!tmp)
		return (1);
	tmp[0] = '\1';
	i = 0;
	while (i < len)
	{
		tmp[i + 1] = s[i];
		i++;
	}
	tmp[i + 1] = '\0';
	free(tok->str);
	tok->str = tmp;
	return (0);
}