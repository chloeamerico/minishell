/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:19:23 by camerico          #+#    #+#             */
/*   Updated: 2025/09/15 17:35:39 by camerico         ###   ########.fr       */
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
		if (value != NULL)
		{
			free(match->value);					//on remplace l'ancienne value par la nouvelle
			match->value = ft_strdup(value);
		}
		else
			return;
	}
	else									//on cree un nv t_env
	{
		new = malloc(sizeof(t_env));
		new->key = ft_strdup(key);
		if (value != NULL)
			new->value = ft_strdup(value);
		else
			new->value = NULL;
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

// static char	*clear_quotes_export(char *str)
// {
// 	char	*new;
// 	int	len;

// 	len = ft_strlen(str);
// 	if(len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\'')))
// 		new = ft_substr(str, 1, len - 1);
// 	else
// 		new = ft_strdup(str);
// 	return(new);
// }

//permet de reetirer les quotes quand on expoort
//par ex: export ""var1=123"" devient var1=123
//pareil pour les simple quotes
static char	*clear_quotes_export(char *str)
{
	char	*new;
	int	len;

	len = ft_strlen(str);
	if(len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\'')))
	{
		while (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\'')))
		{
			new = ft_substr(str, 1, len - 1);
			len -= 2;
			str = new;
		}
	}
	else
		new = ft_strdup(str);
	return(new);
}


//on divise l'arg en key et value a partir du =
static void	divide_key_and_value(char *arg, t_env **env)
{
	int	i;
	char	*key;
	char	*value;
	char	*clean_arg;
	
	clean_arg = clear_quotes_export(arg);
	i = 0;
	while (clean_arg[i] && clean_arg[i] != '=')		//on detecte le =
		i++;
	// if (!clean_arg[i])
	// 	return(free(clean_arg));							//ou exit si il y a pas de = ?
	key = ft_substr(clean_arg, 0, i);
	if (!clean_arg[i])							//si il n'y a pas de =, pas de value
		value = NULL;
	else 
		value = ft_strdup(clean_arg + i + 1);		//on commence au char juste apres le =
	update_or_add_env(key, value, env);
	free(key);
	if (!clean_arg[i])
		free(value);
	free(clean_arg);
}

// //on divise l'arg en key et value a partir du =
// static void	divide_key_and_value(char *arg, t_env **env)
// {
// 	int	i;
// 	char	*key;
// 	char	*value;
// 	char	*clean_arg;
	
// 	clean_arg = clear_quotes_export(arg);
// 	i = 0;
// 	while (clean_arg[i] && clean_arg[i] != '=')		//on detecte le =
// 		i++;
// 	// if (!clean_arg[i])
// 	// 	return(free(clean_arg));							//ou exit si il y a pas de = ?
// 	key = ft_substr(clean_arg, 0, i);
// 	if (!clean_arg[i])							//si il n'y a pas de =, pas de value
// 		value = ft_strdup("NULL");
// 	else 
// 		value = ft_strdup(clean_arg + i + 1);		//on commence au char juste apres le =
// 	update_or_add_env(key, value, env);
// 	free(key);
// 	free(value);
// 	free(clean_arg);
// }

static void	print_env(t_env *env)
{
	t_env	*tmp;
	
	tmp = env;
	while(tmp)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

// //fonction pour trier une version duppliquee de la var d'env par ordre alphabetique
// //on compare la premiere ligne aux autres, si on en trouve une avec le premier char + grand dans la tablea ascii, on le place au top
// static t_env	*tri_env_alpla(t_env *env)
// {
// 	t_env	*tmp;		//ligne de la var d'env qu'on va comparer
// 	t_env	*tmp2;		//ligne a laquelle on va la comparer  
// 	int	i;

// 	i = 0;
// 	tmp = env;
// 	tmp2 = tmp->next;
// 	while(tmp2->next == NULL)
// 	{
// 		if(tmp->key[i] > tmp2->key[i])		//ex : d'abord un 
// 			tmp2 = tmp2->next;
// 		else if(tmp->key[i] == tmp2->key[i])
// 		{
// 			if(ft_strcmp(tmp->key, tmp2->key))
// 		}


		
// 		tmp2 = tmp2->next;
// 		if(tmp->key[i] > tmp2->key[i])
			
// 	}
// }

//fonction pour creer et malloc un nouveau noeud qu'il faudra ensuite remplir
static t_env	*create_new_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if(!new_node)
		return(NULL);

	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;

	return(new_node);
}

// fonction pour inserer de maniere triee
//on compare new_node->key avec sorted_list->key deja triee
//grace a ft_strcmp : si < 0 ==> on doit le placer avant, sinon on continue. Si on est arrive a la fin, on le place en dernier
static void insert_sorted(t_env **sorted_list, t_env *new_node)
{
	t_env	*current;

	current = *sorted_list;
	if(*sorted_list == NULL || ft_strcmp(new_node->key, (*sorted_list)->key) < 0)			//dans le cas ou la sorted list est vide ou qu'il faut placer un node en tete
	{
		new_node->next = *sorted_list;
		*sorted_list = new_node;
		return;
	}
	while(current->next != NULL && ft_strcmp(new_node->key, current->next->key) > 0)		//dans ces cas la on cherche la bonne position
		current = current->next;
	
	//on insert juste apres ce noeud
	new_node->next = current->next;
	current->next = new_node;
}

//fonction pour trier une version duppliquee de la var d'env par ordre alphabetique
//on compare la premiere ligne aux autres, si on en trouve une avec le premier char + grand dans la tablea ascii, on le place au top
//va appeler la fonction pour inserer au bon endroit les noeuds
static t_env	*create_env_sorted(t_env *env)
{
	t_env	*sorted_list = NULL;		//ligne de la var d'env qu'on va comparer
	t_env	*tmp = env;			//pour parcourir l'env
	t_env	*new_node;
	
	while(tmp != NULL)
	{
		new_node = create_new_node(tmp->key, tmp->value);
		if(new_node)
			insert_sorted(&sorted_list, new_node);
		tmp = tmp->next;
	}
	
	return(sorted_list);
}

//lorsqu'on ecirt juste "export", imprimer la var d'env avec exrit "export " avant et dans l'ordre alpha
//1. fonction principale
//va appeler la fonction pour creer une copie triee de l'env
static void	print_env_for_export(t_env *env)
{
	t_env	*sorted_list;
	t_env	*tmp;

	sorted_list = create_env_sorted(env);		//une fois qu'on a creer la liste triee
	tmp = sorted_list;				//il faut l'imprimer avec le "export devant"
	while(tmp)
	{
		printf("export %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}

	free_env(sorted_list);			//puis on free
	
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
	// if (!split[1] && (ft_strcmp(split[0], "env") && !split[1]))		// si on ecrit juste "export" ou "env", afficher la var d'env
	// 	print_env(*env);
	if (!split[1] && (!ft_strcmp(split[0], "env")))		// si on ecrit juste "env", afficher la var d'env
		print_env(*env);
	else if (!split[1] && (!ft_strcmp(split[0], "export")))		// si on ecrit juste "export", afficher la var d'env avec "export" devant trie dans l'ordre
		print_env_for_export(*env);
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


// //on verifie qu'on doit bien faire l'expension, puis appelle toutes els fonctions pour modifier la vaar d'env
// void	export(char **split, t_env **env)
// {
// 	int	i;
	
// 	if (!split || !split[0])
// 		return;
// 	if (ft_strcmp(split[0], "export") != 0 && ft_strcmp(split[0], "env") != 0)		//si le premier mot n'est pas "export" ou "env", on passe a la tokenisation
// 		return;
// 	if (!split[1] || (ft_strcmp(split[0], "env") && !split[1]))		// si on ecrit juste "export" ou "env", afficher la var d'env
// 		print_env(*env);
// 	else
// 	{
// 		i = 1;
// 		while(split[i])
// 		{
// 			divide_key_and_value(split[i], env);
// 			i++;
// 		}	
// 	}
// }


