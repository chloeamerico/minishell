/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chloeamerico <chloeamerico@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 16:05:29 by chloeameric       #+#    #+#             */
/*   Updated: 2025/07/26 00:10:52 by chloeameric      ###   ########.fr       */
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
//dans chaque appel, on va creer un pipe, forker, rediriger les entrées/sorties avec dup2, et executer avec execve.
void	exec_loop(t_cmd *cmd_list, t_env *env)
{
	int	nb_pipe;
	int	pipe_fd[2];		//pour les extremites de lecture et d'ecriture
	char	**envp;
	t_cmd	*cmd = cmd_list;
	pid_t	pid;
	t_pipeline	pipeline;
	
	init_struct_pipe(&pipeline, cmd);
	envp = env_to_array(env);
	nb_pipe = pipeline.nb_cmd - 1;
	if(pipeline.nb_cmd == 1)
		exec_simple_cmd();			//fonction a faire !
	else


	
	while(cmd)
	{
		if(cmd->next)		//si on est pas à la derniere commande, on cree un pipe
			pipe(pipe_fd);
		
		pid = fork();
		
		
		cmd = cmd->next;
	}
}

//permet de savoir combien il y a de commandes, et donc de savoir combien de pipes on va creer.
//on stocke dans dans une structure.
void	init_struct_pipe(t_pipeline	*pipeline, t_cmd *cmd)
{
	int	i;
	t_cmd	*tmp;

	i = 0;
	tmp = cmd;
	while(tmp)
	{
		i++;
		tmp = tmp->next;
	}
	pipeline->nb_cmd = i;
}
