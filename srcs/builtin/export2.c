/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:51:07 by camerico          #+#    #+#             */
/*   Updated: 2025/09/17 17:30:02 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//fonction principale qui va appeler toutes les autres pour export un arg
void	export_one_arg(char *arg, char **env)
{
	char	*key;
	
	key = find_key(arg);
	
	if(already_exist(key, env, arg))		//si la key existe deja
	{
		// if(!ft_strchr(arg, "=")) 	//si l'arg ne possede pas de =
		// 	return;
		update_var_in_env();
	}
	if(add_new_line_in_env(arg, env))
		return;
}

// check si la key existe deja dans la var d'env
// renvoie 0 n'existe pas (il faut la creer)
// renvoie 1 si elle existe deja (il faudra l'update sauf si avant il y avait un = et pas apres)
int	already_exist(char *key, char **env, char *arg)
{
	int	i;
	int	key_len;
	
	i = 0;
	key_len = ft_strlen(key);
	while(env[i])
	{
		if(!ft_strncmp(env[i], key, ft_strlen(key)) && (env[i][key_len] == '=' || env[i][key_len] == '\0' ))		//si la key existe deja, on va l'update
		{
			if(ft_strchr(arg, "="))	
				return(1);
			// return (0);			//a voir si on le laisse
		}
		i++;
	}
	return (0);
}




//on a ajoute un eligne a la fin
int	add_new_line_in_env(char *arg, char ***env)
{
	int	i;

	i = 0;
	while(*env[i])
		i++;
	*env[i] = ft_strdup(arg);
	if (*env[i])
		return (1);
	*env[i + 1] = NULL;
	return (0);
}

//Trouver l'index de la variable à modifier
//puis Libérer l'ancienne chaîne (free(env[index]))
//Créer la nouvelle chaîne ("USER=jane")

