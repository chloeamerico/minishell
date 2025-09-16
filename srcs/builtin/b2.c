/* ************************************************************************** */
/*   export_env.c - Implémentation complète d'export et env                  */
/* ************************************************************************** */

#include "minishell.h"



int	ft_setenv(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	if (!env || !key)  //  AJOUT: Vérification des paramètres
		return (1);

	current = *env;
	while (current)
	{
		//  FIX PRINCIPAL: Vérifier que current->key n'est pas NULL
		if (current->key && !ft_strcmp(current->key, key))
		{
			if (current->value)
				free(current->value);
			current->value = ft_strdup(value ? value : "");
			return (0);
		}
		current = current->next;
	}

	/* Créer nouveau nœud */
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (1);
	
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value ? value : "");
	
	//  AJOUT: Vérifier que les malloc ont réussi
	if (!new_node->key || !new_node->value)
	{
		if (new_node->key)
			free(new_node->key);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
		return (1);
	}
	
	new_node->next = *env;
	*env = new_node;
	return (0);
}

