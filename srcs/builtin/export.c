/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:17:16 by camerico          #+#    #+#             */
/*   Updated: 2025/09/17 17:42:58 by camerico         ###   ########.fr       */
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

- gerer les quotes
		
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

//fonction pourecrire l'env avec le declare -x devant
int	print_export(char **env)
{
	int	i;
	char	*key;
	char	*value;


	i = 0;
	while(env[i])
	{
		key = find_key(env[i]);
		value = find_value(env[i]);
		if (!value)						//si pas de value , juste export VAR
			printf("%s\n", key);
		else
			printf("declare -x %s=\"%s\"\n", env[i]);
		free(key);
		if (value)
			free(value);
		i++;
	}
	return (0);
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




char *find_key(char *arg)
{
	int	i;
	int j;
	char *key;

	i = 0;
	j = 0;
	while(arg[i] != '=' && arg[i])
		i++;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		return(NULL);
	while(j < i)
	{
		key[j] = arg[j];
		j++;
	}
	key[j] = '\0';
	return(key);
}

char	*find_value(char *arg)
{
	int	i;
	char	*value;
	
	i = 0;
	while(arg[i] != '=' && arg[i])
		i++;
	if (!arg[i])				//si pas de =, pas de value;
		return(NULL);
	i++;
	value = malloc(sizeof(char) * (ft_strlen(arg + i) + 1));
	if(!value)
		return(NULL);
	ft_strcpy(value, arg + i);
	return (value);
}
