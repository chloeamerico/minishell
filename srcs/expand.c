/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:59:26 by camerico          #+#    #+#             */
/*   Updated: 2025/07/07 20:04:08 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//on parcourt la liste, on check si le type est une CMD ou WRD ou FD
void	expand_tokens(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*tmp;
	char *expanded;

	tmp = tokens;
	while(tmp)
	{
		if(tmp->type == CMD || tmp->type == WRD || tmp->type == FD)
		{
			if(!check_if_expand(tmp->str))		//si il y a un $
			{
				expanded = build_expand(tmp->str, env, exit_status);
				free(tmp->str);
				tmp->str = expanded;
			}
		}
		tmp = tmp->next;
	}
}

//fonction qui va verifier si il y a un $ dans la chaine du maillon
//return(1) si PAS de $
//return (0) s'il y a un $
int	check_if_expand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if(str[i] == '$')		// il n'y a pas de $
			return (0);
		i++;
	}
	return (1);
}

// //on va construire l'expand
// char	*build_expand(char *str, t_env *env, int exit_status)
// {
// 	int	quotes;
// 	int	i = 0;
// 	char *new_str;
// 	char *var_name;
// 	char *value;

// 	new_str = ft_strdup(""); 	//on duplique une strig vide (avec malloc)
// 	quotes = STATE_NONE;
// 	while(str[i])
// 	{
// 		quote_state(str[i], &quotes);
// 		while(str[i] != "$")
// 		{
// 			if (str[i] != 34)
// 				new_str[i] = str[i];
// 			i++;
// 		}
// 		i++;
// 		quote_state(str[i], quotes);
// 		if (quotes != STATE_SINGLE)
// 			extract_var(&str[i], env);		//on commence a str[i] jusqu'a la fin
		
// 	}
// 	return (new_str);
// }

//on va construire l'expand
char	*build_expand(char *str, t_env *env, int exit_status)
{
	int	quotes;
	int	i = 0;
	char *new_str;
	char *var_name;
	char *value;
	char tmp[2];		//buffer temp pour chaque char, qu'on va ensuite strjoin a new_str

	new_str = ft_strdup(""); 	//on duplique une strig vide (avec malloc)
	quotes = STATE_NONE;
	while(str[i])
	{
		quote_state(str[i], quotes);
		if (str[i] == '$' && quotes != STATE_SINGLE)	// i le char est un $ en dehors des quotes simples, donc a expand
		{
			i++;	//pour sauter le char $
			var_name = extract_var(&str[i], &i);		//on recup VAR dans $VAR + on avance i
			value = get_env_value(var_name, env, exit_status);	//on cherche sa value dans l'env
			new_str = ft_strjoin_free(new_str, value);
			free(value);
		}
		else
		{
			tmp[0] = str[i];
			tmp[1] = '\0';
			new_str = ft_strjoin_free(new_str, tmp);
			i++;
		}
		
	}
	return (new_str);
}

//on extrait le nom de la var
//ex : on extrait $VAR dans abc$VAR
//attention aux cas particuliers (le $!, $$ et $0)
//on avance l'indexe i
char	*extract_var(char *str, int *i)
{
	int	start;
	char	*var;
	int	len = 0;

	start = 0;
	if (str[0] == '?')
	{
		(*i)++;
		return(ft_strdup("?"));
	}
	while(ft_isalnum(str[len]) && str[len] == '_')
		len++;
	(*i) += len;
	var = ft_substr(str, i, len);
	return(var);
}


//on va chercher dans la var d'env quelle value correspond a la key
char	*get_env_value(char *var, t_env *env, int exit_status)
{
	t_env	*tmp;

	tmp = env;
	if (!strncmp(var, "?", 1))
		return(ft_itoa(exit_status));
	while (tmp)
	{
		if(ft_strcmp(var, tmp->key))
			return(ft_strdup(tmp->value));
		else
			tmp = tmp->next;
	}
	if (!env->next)		//si ca n'existe pas, return une erreur
		return (NULL);
}
