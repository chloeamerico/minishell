/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:17:16 by camerico          #+#    #+#             */
/*   Updated: 2025/09/16 19:13:18 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*A FAIRE 

les char autorises :
- Doit commencer par une lettre (a–z, A–Z) ou un underscore (_).
- Les caractères suivants peuvent être :
		lettres (a–z, A–Z),
		chiffres (0–9),
		underscore (_) 
		
print_export(env);		//print l'env dans l'ordre avec declare -x avant

add_new_line_in_env;

update_env_with_new_key(arg, env);

*/


int	ft_export(char **args, char **env)
{
	int	i;
	
	if (!args || !args[0])
		return (1);
	if (!args[1] && (!ft_strcmp(args[0], "export")))		// si on ecrit juste "export", afficher la var d'env avec "declare -x" devant trie dans l'ordre
		print_export(env);
	else
	{
		i = 1;
		while(args[i])
		{
			export_one_arg(args[i], env);
			i++;
		}
	}
}

//fonction principale qui va appeler toutes les autres pour export un arg
void	export_one_arg(char *arg, char **env)
{
	char	*key;
	
	key = find_key(arg);
	
	if(already_exist(key, env))		//si la key existe deja
	{
		if(!ft_strchr(arg, "=")) 	//si l'arg ne possede pas de =
			return;
	}
	if(add_new_line_in_env(arg, env))
		return;
}

// check si la key existe deja dans la var d'env
// renvoie 0 n'existe pas (il faut la creer)
// renvoie 1 si elle existe deja (il faudra l'update sauf si avant il y avait un = et pas apres)
// int	already_exist(char *arg, char **env)
// {
// 	int	i;
	
// 	i = 0;
// 	while(env[i])
// 	{
// 		if(!ft_strncmp(env[i], arg, ft_strlen(arg)));		//si la key existe deja
// 			return(1);
// 		i++;
// 	}
// 	return (0);
// }

int	already_exist(char *key, char **env)
{
	int	i;
	
	i = 0;
	while(env[i])
	{
		if(!ft_strncmp(env[i], key, ft_strlen(key)));		//si la key existe deja, on va l'update
		{
			if(ft_strchr(key, "="))						//si l'arg existe deja dans la var d'env et possede un =, on va la free puis la rajouter apres
			{
				free(env[i]);
				return(1);
			}
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

char *find_key(char *arg)
{
	int	i;
	int j;
	char *key;

	i = 0;
	j = 0;
	while(arg[i] != "=" && arg[i])
		i++;
	if (!arg[i])		//si on est arrive a la fin
		return(arg);
	else
	{
		while(i < j)
		{
			key[j] = arg[j];
			j++;
		}
	}
	return(key);
}