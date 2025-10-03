/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:59:26 by camerico          #+#    #+#             */
/*   Updated: 2025/10/02 12:20:12 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//fonction qui va verifier si il y a un $ dans la chaine du maillon
//return (1) si PAS de $
//return (0) s'il y a un $
static int	check_if_expand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

//on extrait le nom de la var
//ex : on extrait $VAR dans abc$VAR
//attention aux cas particuliers (le $!, $$ et $0)
//on avance l'indexe iget_env_value
static char	*extract_var(char *str, int *i)
{
	char	*var;
	int		len;

	len = 0;
	if (str[0] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (!ft_isalnum(str[0]) && str[0] != '_' && str[0] != '?')
		return (ft_strdup(""));
	while (ft_isalnum(str[len]) || str[len] == '_')
		len++;
	(*i) += len;
	var = ft_substr(str, 0, len);
	return (var);
}

//on va construire l'expand
//AVANT DE REDUIRE
// static char	*build_expand(char *str, t_env *env, int exit_status)
// {
// 	int		quotes;
// 	int		i;
// 	char	*new_str;
// 	char	*var_name;
// 	char	*value;
// 	char	tmp[2];

// 	i = 0;
// 	new_str = ft_strdup("");
// 	quotes = STATE_NONE;
// 	while (str[i])
// 	{
// 		quote_state(str[i], &quotes);
// 		if (str[i] == '$' && quotes != STATE_SINGLE)
// 		{
// 			i++;
// 			var_name = extract_var(&str[i], &i);
// 			value = get_env_value(var_name, env, exit_status);
// 			new_str = ft_strjoin_free(new_str, value);
// 			free(var_name);
// 			free(value);
// 		}
// 		else
// 		{
// 			tmp[0] = str[i];
// 			tmp[1] = '\0';
// 			new_str = ft_strjoin_free(new_str, tmp);
// 			i++;
// 		}
// 	}
// 	return (new_str);
// }
static char	*variable(char *str, t_expand *expand, t_env *env, int exit_status)
{
	char	*var_name;
	char	*value;
	char	*result;

	expand->i++;
	var_name = extract_var(&str[expand->i], &expand->i);
	value = get_env_value(var_name, env, exit_status);
	result = ft_strdup(value);
	expand->new_str = ft_strjoin_free(expand->new_str, result);
	free(var_name);
	free(value);
	return (result);
}

static char	*build_expand(char *str, t_env *env, int exit_status)
{
	int			quotes;
	t_expand	expand;
	char		tmp[2];
	char		*var_result;

	expand.i = 0;
	expand.new_str = ft_strdup("");
	quotes = STATE_NONE;
	while (str[expand.i])
	{
		quote_state(str[expand.i], &quotes);
		if (str[expand.i] == '$' && quotes != STATE_SINGLE)
		{
			var_result = variable(str, &expand, env, exit_status);
			free(var_result);
		}
		else
		{
			tmp[0] = str[expand.i];
			tmp[1] = '\0';
			expand.new_str = ft_strjoin_free(expand.new_str, tmp);
			expand.i++;
		}
	}
	return (expand.new_str);
}

//on va chercher dans la var d'env quelle value correspond a la key
char	*get_env_value(char *var, t_env *env, int exit_status)
{
	t_env	*tmp;

	tmp = env;
	if (!strncmp(var, "?", 1))
		return (ft_itoa(exit_status));
	while (tmp)
	{
		if (!ft_strcmp(var, tmp->key))
			return (ft_strdup(tmp->value));
		else
			tmp = tmp->next;
	}
	return (ft_strdup(""));
}

//on parcourt la liste, on check si le type est une CMD ou WRD ou FD
void	expand_tokens(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*tmp;
	char	*expanded;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == CMD || tmp->type == WRD || tmp->type == FD)
		{
			if (check_if_expand(tmp->str))
			{
				expanded = build_expand(tmp->str, env, exit_status);
				free(tmp->str);
				tmp->str = expanded;
			}
		}
		tmp = tmp->next;
	}
}
