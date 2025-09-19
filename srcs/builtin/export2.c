/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:51:07 by camerico          #+#    #+#             */
/*   Updated: 2025/09/19 14:48:09 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//fonction qui va creer une nouvelle variable dans l'env
static char **add_new_line_in_env(char *arg, char **env)
{
	int	i;
	int	count;
	char **new_env;

	count = 0;
	while(env[count])
		count++;
	i = 0;
	new_env = malloc(sizeof(char*) * (count + 2));		//+2 car +1 pour la ligne supp et +1 pour le NULL
	if (!new_env)
		return(NULL);
	i = 0;
	while(i < count)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[count] = ft_strdup(arg);
	if(!new_env)
	{
		free(new_env);
		return(NULL);
	}
	new_env[count + 1] = NULL;
	free(env);
	return(new_env);
}

//fonction qui va modifier une variable dans l'env
//Trouver l'index de la variable à modifier
//puis Libérer l'ancienne chaîne (free(env[index]))
//Créer la nouvelle chaîne ("USER=jane")
static int	update_var_in_env(char *arg, char **env, int index)
{
	if(index >= 0)
		free(env[index]);
	env[index] = malloc(sizeof(char) * (ft_strlen(arg) + 1));
	if(!env[index])
		return(1);
	ft_strcpy(env[index], arg);
	return (0);
}

//fonction principale qui va appeler toutes les autres pour export un arg
void	export_one_arg(char *arg, char ***env)
{
	char	*key;
	int	index;
	
	key = find_key(arg);
	if(!key)
		return;
	
	index = find_index(key, *env);
	if(index != -1)			//si la key existe deja
	{
		if(!ft_strchr(arg, '=')) 	//si l'arg ne possede pas de =, on ne fait rien
		{
			free(key);
			return;
		}
		update_var_in_env(arg, *env, index);
		return;
	}
	else
	{
		*env = add_new_line_in_env(arg, *env);
		free(key);
	}
}

// // check si la key existe deja dans la var d'env
// // renvoie 0 n'existe pas (il faut la creer)
// // renvoie 1 si elle existe deja (il faudra l'update sauf si avant il y avait un = et pas apres)
// int	already_exist(char *key, char **env, char *arg)
// {
// 	int	i;
// 	int	key_len;
	
// 	i = 0;
// 	key_len = ft_strlen(key);
// 	while(env[i])
// 	{
// 		if(!ft_strncmp(env[i], key, ft_strlen(key)) && (env[i][key_len] == '=' || env[i][key_len] == '\0' ))		//si la key existe deja, on va l'update
// 		{
// 			if(ft_strchr(arg, "="))
// 				return(1);
// 			return (0);			//a voir si on le laisse
// 		}
// 		i++;
// 	}
// 	return (0);
// }




// //on a ajoute un eligne a la fin
// int	add_new_line_in_env(char *arg, char ***env)
// {
// 	int	i;

// 	i = 0;
// 	while(*env[i])
// 		i++;
// 	*env[i] = ft_strdup(arg);
// 	if (*env[i])
// 		return (1);
// 	*env[i + 1] = NULL;
// 	return (0);
// }


