/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:17:16 by camerico          #+#    #+#             */
/*   Updated: 2025/09/22 18:17:03 by camerico         ###   ########.fr       */
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

char *find_key(char *arg)
{
	int	i;
	int j;
	char *key;

	i = 0;
	j = 0;
	if (!arg)
		return (NULL);
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

int	find_index(char *key, char **env)
{
	int	i;

	i = 0;
	while(env[i])
	{
		if(!ft_strncmp(env[i], key, ft_strlen(key)) && (env[i][ft_strlen(key)] == '=' || env[i][ft_strlen(key)] == '\0' ))
			return(i);
		i++;
	}
	return(-1);
}

static int print_export(char **env)
{
	int i; 
	int j;

	i = 0;
	while (env && env[i])
	{
		write(1, "declare -x ", 11);
		j = 0;
		while (env[i][j] && env[i][j] != '=')
		{
			write(1, &env[i][j], 1);
			j++;
		}
		if (env[i][j] == '=' && env[i][j + 1])
		{
			write(1, "=\"", 2); 
			j++;
			while (env[i][j])
			{
				write(1, &env[i][j], 1);
				j++;
			}
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

int	ft_export(char **args, char ***env)
{
	int	i;
	
	if (!args || !args[0])
		return (1);
	if (!args[1] && (!ft_strcmp(args[0], "export")))		// si on ecrit juste "export", afficher la var d'env avec "declare -x" devant trie dans l'ordre
		print_export(*env);
	else
	{
		i = 1;
		while(args[i])
		{
			export_one_arg(args[i], env);
			i++;
		}
	}
	return (0);
}




