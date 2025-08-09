/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:48:07 by camerico          #+#    #+#             */
/*   Updated: 2025/08/09 20:00:07 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* UTILISATION DE 2 PIPES :

cmd1 | cmd2 | cmd3 | cmd4

Étapes :
1. cmd1 écrit dans pipe1[1] → cmd2 lit pipe1[0]
2. cmd2 écrit dans pipe2[1] → cmd3 lit pipe2[0] 
3. cmd3 écrit dans pipe1[1] → cmd4 lit pipe1[0]
4. etc...

current_pipe = cmd_index % 2;
	cmd_index 0 → current_pipe = 0 (pipe1)
	cmd_index 1 → current_pipe = 1 (pipe2)  
	cmd_index 2 → current_pipe = 0 (pipe1)
	cmd_index 3 → current_pipe = 1 (pipe2)

le pipe a deux extremites :
pipefd[0] -> extremite de lecture du pipe
pipefd[1] -> extremite d'écriture du pipe

Commande IMPAIRE utilise pipe1, PAIRE utilise pipe2

on alloue 1 PID par process enfant

pour chaque iteration de la boucle on va :
        1. Créer le pipe si nécessaire
        2. Fork le processus
        3. Configurer les redirections
        4. Exécuter la commande
        5. Nettoyer dans le parent

*/


//pour initialiser la structure
void	init_pipeline(t_pipeline *pipeline)
{
	pipeline->current_pipe = 0;
	pipeline->prev_pipe = -1;		//au deb pas de prev pipe

	//on initialise tous les descripteurs a -1 car fermes au debut
	
	pipeline->pipefd1[0] = pipeline->pipefd1[1] = -1;		
	pipeline->pipefd2[0] = pipeline->pipefd2[1] = -1;
}

//compter les cmd dans la liste
//permet de savoir combien il y a de commandes, et donc de savoir combien de pipes on va creer.
//on stocke dans dans une structure.
void	count_cmd(t_pipeline *pipeline, t_cmd *cmd)
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

// une commande = 1 processus
// on a besoin de tous les PIDs pour faire un waitpid() a la fin
//on alloue un tableau pour stocker les PID
int	pid_array(t_pipeline *pipeline, t_cmd *cmd, pid_t *pids)
{
	int	cmd_count;

	count_cmd(pipeline, cmd);
	cmd_count = pipeline->nb_cmd;
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return(perror("malloc"), 1);
	return (0);
}

//boucle principale pour l'exec
int	exec_pipeline(t_cmd *cmd_list, t_env *env)
{
	t_pipeline	pipeline;
	t_cmd	*current_cmd;
	pid_t	*pids;
	int	cmd_index;
	
	if (!cmd_list)
		return(1);
	
	if (!cmd_list->next)			//cas de 1 seule cmd sans pipe
		exec_single_without_pipe();		// a faire
	
	pid_array(&pipeline, cmd_list, *pids);		//1ere partie de l'initialisation
	init_pipeline(&pipeline);					//2eme partie de l'initialisation
		
	current_cmd = cmd_list;
	while(current_cmd)
	{
		if(current_cmd->next)
			create_pipe(&pipeline);
	}

	//boucle
}

//fonction pour creer les pipes
int	create_pipe(t_pipeline *pipeline)
{
	//ajouter une condition pour que ca ne le fasse pas si on est a la derniere cmd
	
	if (pipeline->current_pipe == 0)		//si on est dans le pipe1
	{
		if (pipeline->pipefd1[0] == -1)		//si le pipe n'a jamais ete cree
		{
			if(pipe(pipeline->pipefd1) == -1)
				return(perror("creation pipe 1 failed"), 1);
		}
	}
	else		//on est dans le pipe 2
	{
		if (pipeline->pipefd2[0] == -1)
		{
			if (pipe(pipeline->pipefd2) == -1)
				return(perror("creation pipe 1 failed"), 1);
		}
	}
	return (0);
}