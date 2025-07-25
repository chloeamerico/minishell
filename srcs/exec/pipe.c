/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chloeamerico <chloeamerico@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 16:05:29 by chloeameric       #+#    #+#             */
/*   Updated: 2025/07/24 21:08:22 by chloeameric      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// le pipe a deux extremites : 
// pipefd[0] -> extremite de lecture du pipe
// pipefd[1] -> extremite d'écriture du pipe

#include "minishell.h"

int	size_env(t_env *env)
{
	int	i;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while(tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return(i);
}

//on passe la variable d'env d'une liste chainee à un double tab pour que ca puisse etre utilisé dans execve
char **env_to_array(t_env *env)
{
	int	i;
	int	size;
	t_env	*tmp;
	char	**tab;

	i = 0;
	tmp = env;
	size = size_env(env);
	tab = malloc(sizeof(char *) * (size + 1));
	if(!tab)
		return(NULL);
	while(tmp)
	{
		tab[i] = ft_strjoin(tmp->key, "=");
		tab[i] = ft_strjoin_free(tab[i], tmp->value);
		tmp = tmp->next;
		i++;
	}
	tab[i] = NULL;
	return(tab);
}
 
//fonction qui va boucler pour appliquer a tous les t_cmd
void	exec_loop(t_cmd *cmd_list, t_env *env)
{
	char	**envp;
	t_cmd	*cmd = cmd_list;
	int	pipe[2][2];
	
	envp = env_to_array(env);
	while(cmd)
	(
		if(cmd->next)
		
	)
}