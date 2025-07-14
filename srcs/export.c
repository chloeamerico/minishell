/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:19:23 by camerico          #+#    #+#             */
/*   Updated: 2025/07/14 18:20:10 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//creer la structure
//initialiser/detecter key/value depuis envp
//gerer export , si la variable existe deja on modifie sa valeur, sinon on la creer en lui associant la valeur
//quand on appelle avec le $, faire une fonction qui parcours la liste et retourne la value associee a key


//fonction qui va regarder si la var existe deja dans l'env:
// si oui, modifie la valeur associee
// si non cree la variable et ajoute sa valeur associee 


//on parcours la liste en env pour voir si key existe
static t_env	*check_in_env(char *key, t_env *env)
 {
	while(env)
	{
		if (ft_strcmp(env->key, key) == 0)		// si key deja presente, return (1)
			return(env);
		env = env->next;
	}
	return (NULL);									// si key n'est pas present, return (0);
 }


//si la var n'existe pas on l'ajoute
//on lui associe la valeur ou on la met a jour si elle existe deja
static void	update_or_add_env(char *key, char *value, t_env **env)
{
	t_env	*match;
	t_env	*new;
	t_env	*tmp;
	
	match = check_in_env(key, *env);			//on check si il y a eu un match avec les key deja dans la var d'env
	if(match)
	{
		free(match->value);					//on remplace l'ancienne value par la nouvelle
		match->value = ft_strdup(value);
	}
	else									//on cree un nv t_env
	{
		new = malloc(sizeof(t_env));
		new->key = ft_strdup(key);
		new->value = ft_strdup(value);
		new->next = NULL;
		if (*env == NULL)
			*env = new;
		else
		{
			tmp = *env;
			while(tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
}

//on divise l'arg en key et value a partir du =
static void	divide_key_and_value(char *arg, t_env **env)
{
	int	i;
	char	*key;
	char	*value;
	
	i = 0;
	while (arg[i] && arg[i] != '=')		//on detecte le =
		i++;
	if (!arg[i])
		return;							//ou exit si il y a pas de = ?
	key = ft_substr(arg, 0, i);
	value = ft_strdup(arg + i + 1);		//on commence au char juste apres le =
	update_or_add_env(key, value, env);
	free(key);
	free(value);
}






//parcours les t_token de type CMD ou ARG
//cherche les $VAR , $?
// remplace les $VAR par leur valeur
//remplace $? par le code de sortie precedent
//ne pas expand les simple quotes



//on verifie qu'on doit bien faire l'expension, puis appelle toutes els fonctions pour modifier la vaar d'env
void	export(char **split, t_env **env)
{
	int	i;
	
	if (!split || !split[0])
		return;
	if (ft_strcmp(split[0], "export") != 0 && ft_strcmp(split[0], "env") != 0)		//si le premier mot n'est pas "export" ou "env", on passe a la tokenisation
		return;
	if (!split[1] || (ft_strcmp(split[0], "env") && !split[1]))		// si on ecrit juste "export" ou "env", afficher la var d'env
		return;		// return pour pouvoir le tester, mais ensuite mettre et coder la fonction print_env;
	else
	{
		i = 1;
		while(split[i])
		{
			divide_key_and_value(split[i], env);
			i++;
		}	
	}
}


