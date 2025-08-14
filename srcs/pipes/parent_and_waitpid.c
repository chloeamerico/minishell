/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_and_waitpid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:49:40 by camerico          #+#    #+#             */
/*   Updated: 2025/08/14 15:50:52 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*

Rôle du processus PARENT :

	Stocker le PID de l'enfant (pour faire waitpid() plus tard)
	Fermer les descripteurs appropriés (pour que les pipes fonctionnent)
	Continuer la boucle pour la commande suivante


	- si on est PAS a la PREMIERE commande, on ferme de cote lecture du pipe precedent
	- si ce n'est PAS la DERNIERE cmd, on ferme le cote d'ecrture du pipe actuel
	+ remettre a -1 les descripteurs fermes


*/

void	parent_process(t_pipeline *pipeline, int cmd_index)
{

	// pipeline->current_pipe = cmd_index % 2;			//on met a jour la struct
	// if(cmd_index != 0)
	// 	pipeline->prev_pipe = (cmd_index - 1) % 2;
	// else
	// 	pipeline->prev_pipe = -1;


	if (cmd_index > 0)
	{
		if(pipeline->prev_pipe == 0)
		{
			if(pipeline->pipefd1[0] != -1)
				close(pipeline->pipefd1[0]);
			pipeline->pipefd1[0] = -1;
		}
		else
		{
			if(pipeline->pipefd2[0] != -1)
				close(pipeline->pipefd2[0]);
			pipeline->pipefd2[0] = -1;
		}
	}
	if (cmd_index < (pipeline->nb_cmd - 1))
	{
		if (pipeline->current_pipe == 0)
		{
			if(pipeline->pipefd1[1] != -1)
				close(pipeline->pipefd1[1]);
			pipeline->pipefd1[1] = -1;
		}
		else
		{
			if(pipeline->pipefd2[1] != -1)
				close(pipeline->pipefd2[1]);
			pipeline->pipefd2[1] = -1;
		}
	}
}




/* Gestion des signaux :

	WIFEXITED(status) : Processus terminé normalement
	WEXITSTATUS(status) : Code de retour (0-255)
	WIFSIGNALED(status) : Processus tué par un signal
	WTERMSIG(status) : Numéro du signal --> si tue par un signal , le code de sortie est 128 + num du signal*/

int	wait_children_pid(t_pipeline *pipeline, pid_t *pid)
{
	int	i;
	int	exit_status;
	int	last_exit_status;

	i = 0;
	last_exit_status = 0;
	while(i < pipeline->nb_cmd)
	{
		if(waitpid(pid[i], &exit_status, 0) == -1)
			perror("waitpid");
		else					//on recup le code de sortie de la derniere cmd
		{
			if (i == pipeline->nb_cmd - 1)
			{
				if(WIFEXITED(exit_status))		//si tout s'est fini normalemt
					last_exit_status = WEXITSTATUS(exit_status);		//auel est le code de retour
				else if (WIFSIGNALED(exit_status))		//est ce qu'il a ete tue par un signal (ex: ctl + C)
					last_exit_status = 128 + WTERMSIG(exit_status);
			}
		}
		i++;
	}
	return (last_exit_status);
}
