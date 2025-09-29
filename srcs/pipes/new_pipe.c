/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:48:07 by camerico          #+#    #+#             */
/*   Updated: 2025/09/29 16:51:23 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"

// /* UTILISATION DE 2 PIPES :

// cmd1 | cmd2 | cmd3 | cmd4

// Étapes :
// 1. cmd1 écrit dans pipe1[1] → cmd2 lit pipe1[0]
// 2. cmd2 écrit dans pipe2[1] → cmd3 lit pipe2[0]
// 3. cmd3 écrit dans pipe1[1] → cmd4 lit pipe1[0]
// 4. etc...

// current_pipe = cmd_index % 2;
// 	cmd1	cmd_index 0 → current_pipe = 0 (pipe1)
// 	cmd2	cmd_index 1 → current_pipe = 1 (pipe2)
// 	cmd3	cmd_index 2 → current_pipe = 0 (pipe1)
// 	cmd4	cmd_index 3 → current_pipe = 1 (pipe2)

// le pipe a deux extremites :
// pipefd[0] -> extremite de lecture du pipe
// pipefd[1] -> extremite d'écriture du pipe

// Commande IMPAIRE utilise pipe1, PAIRE utilise pipe2

// on alloue 1 PID par process enfant

// pour chaque iteration de la boucle on va :
// 		1. Créer le pipe si nécessaire
// 		2. Fork le processus
// 		3. Configurer les redirections
// 		4. Exécuter la commande
// 		5. Nettoyer dans le parent

// */

// // pour initialiser la structure
// static void init_pipeline(t_pipeline *pipeline)
// {
// 	pipeline->current_pipe = -1;
// 	pipeline->prev_pipe = -1;

// 	// on initialise tous les descripteurs a -1 car fermes au debut

// 	pipeline->pipefd1[0] = pipeline->pipefd1[1] = -1;
// 	pipeline->pipefd2[0] = pipeline->pipefd2[1] = -1;
// }

// // compter les cmd dans la liste
// // permet de savoir combien il y a de commandes, et donc de savoir combien de pipes on va creer.
// // on stocke dans dans une structure.
// static void count_cmd(t_pipeline *pipeline, t_cmd *cmd)
// {
// 	int i;
// 	t_cmd *tmp;

// 	i = 0;
// 	tmp = cmd;
// 	while (tmp)
// 	{
// 		i++;
// 		tmp = tmp->next;
// 	}
// 	pipeline->nb_cmd = i;
// }

// // une commande = 1 processus
// // on a besoin de tous les PIDs pour faire un waitpid() a la fin
// // on alloue un tableau pour stocker les PID
// pid_t *pid_array(t_pipeline *pipeline, t_cmd *cmd)
// {
// 	int cmd_count;
// 	pid_t *pids;

// 	count_cmd(pipeline, cmd);
// 	cmd_count = pipeline->nb_cmd;
// 	pids = malloc(sizeof(pid_t) * cmd_count);
// 	if (!pids)
// 		return (perror("malloc"), NULL);
// 	return (pids);
// }

// // NEW VERSION
// // fonction pour creer les pipes
// static int create_pipe(t_pipeline *pipeline)
// {
// 	// ajouter une condition pour que ca ne le fasse pas si on est a la derniere cmd

// 	if (pipeline->current_pipe == 0) // si on est dans le pipe1
// 	{
// 		if (pipeline->pipefd1[0] != -1) // si l'ancien pipe 1 existe deja, on le ferme
// 		{
// 			close(pipeline->pipefd1[0]);
// 			close(pipeline->pipefd1[1]);
// 		}
// 		if (pipe(pipeline->pipefd1) == -1)
// 			return (perror("creation pipe 1 failed"), 1);
// 	}
// 	else // on est dans le pipe 2
// 	{
// 		if (pipeline->pipefd2[0] != -1)
// 		{
// 			close(pipeline->pipefd2[0]);
// 			close(pipeline->pipefd2[1]);
// 		}
// 		if (pipe(pipeline->pipefd2) == -1)
// 			return (perror("creation pipe 2 failed"), 1);
// 	}
// 	return (0);
// }

// // boucle principale pour l'exec
// int exec_pipeline(t_cmd *cmd_list, t_env *env)
// {
// 	t_pipeline pipeline;
// 	t_cmd *current_cmd;
// 	pid_t *pids;
// 	int cmd_index = 0;
// 	int exit_status = 0;

// 	if (!cmd_list)
// 		return (1);

// 	if (!cmd_list->next) // cas de 1 seule cmd sans pipe
// 	{
// 		if (one_cmd_without_pipe(cmd_list, env))
// 			return (1);
// 		else
// 			return (0);
// 	}
// 	pids = pid_array(&pipeline, cmd_list); // 1ere partie de l'initialisation
// 	if (!pids)
// 		return (1);
// 	init_pipeline(&pipeline); // 2eme partie de l'initialisation

// 	current_cmd = cmd_list;

// 	if (loop_pipe(&pipeline, cmd_index, current_cmd, pids, env))
// 		return (free(pids), 1);

// 	exit_status = wait_children_pid(&pipeline, pids);
// 	free(pids);
// 	return (exit_status);
// }

// // dans le cas ou on a pas de pipe (donc 1 seule cmd), on l'execute dans un process enfant
// int one_cmd_without_pipe(t_cmd *cmd_list, t_env *env)
// {
// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		exec_simple_cmd(cmd_list, env); // L'enfant exécute et exit
// 	}
// 	else if (pid > 0)
// 	{
// 		int status;
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			return (WEXITSTATUS(status));
// 		else if (WIFSIGNALED(status))
// 			return (128 + WTERMSIG(status));
// 		return (1);
// 	}
// 	else
// 	{
// 		perror("fork");
// 		return (1);
// 	}
// 	return (0);
// }

// // execution de la boucle pour creer les pipes, et executer les commandes
// int loop_pipe(t_pipeline *pipeline, int cmd_index, t_cmd *current_cmd, pid_t *pids, t_env *env)
// {
// 	while (current_cmd)
// 	{
// 		pipeline->current_pipe = cmd_index % 2; // on met a jour la struct
// 		if (cmd_index != 0)
// 			pipeline->prev_pipe = (cmd_index - 1) % 2;
// 		else
// 			pipeline->prev_pipe = -1;

// 		if (current_cmd->next)
// 			create_pipe(pipeline);

// 		pids[cmd_index] = fork();

// 		if (pids[cmd_index] == -1) // erreur
// 		{
// 			perror("error : fork");
// 			close_all_pipes(pipeline);
// 			while (cmd_index > 0)
// 			{
// 				cmd_index--;
// 				waitpid(pids[cmd_index], NULL, 0);
// 			}
// 			// free(pids);
// 			return (1);
// 		}
// 		else if (pids[cmd_index] == 0) // on est dans le processus ENFANT
// 		{
// 			child_process(cmd_index, pipeline, current_cmd, env, pids);
// 			// free(pids);
// 		}
// 		else // on est dans le processus PARENT
// 		{
// 			parent_process(pipeline, cmd_index);
// 		}
// 		current_cmd = current_cmd->next;
// 		cmd_index++;
// 	}
// 	// if (pids)
// 	// 	free(pids);
	
// 	return (0);
// }

/* ************************************************************************** */

#include "minishell.h"

static void init_pipeline(t_pipeline *pipeline)
{
	pipeline->current_pipe = -1;
	pipeline->prev_pipe = -1;
	pipeline->pipefd1[0] = pipeline->pipefd1[1] = -1;
	pipeline->pipefd2[0] = pipeline->pipefd2[1] = -1;
}

static void count_cmd(t_pipeline *pipeline, t_cmd *cmd)
{
	int i;
	t_cmd *tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	pipeline->nb_cmd = i;
}

pid_t *pid_array(t_pipeline *pipeline, t_cmd *cmd)
{
	int cmd_count;
	pid_t *pids;

	count_cmd(pipeline, cmd);
	cmd_count = pipeline->nb_cmd;
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (perror("malloc"), NULL);
	return (pids);
}

static int create_pipe(t_pipeline *pipeline)
{
	if (pipeline->current_pipe == 0)
	{
		if (pipeline->pipefd1[0] != -1)
		{
			close(pipeline->pipefd1[0]);
			close(pipeline->pipefd1[1]);
		}
		if (pipe(pipeline->pipefd1) == -1)
			return (perror("creation pipe 1 failed"), 1);
	}
	else
	{
		if (pipeline->pipefd2[0] != -1)
		{
			close(pipeline->pipefd2[0]);
			close(pipeline->pipefd2[1]);
		}
		if (pipe(pipeline->pipefd2) == -1)
			return (perror("creation pipe 2 failed"), 1);
	}
	return (0);
}

int exec_pipeline(t_cmd *cmd_list, t_env *env)
{
	t_pipeline pipeline;
	t_cmd *current_cmd;
	pid_t *pids;
	int cmd_index = 0;
	int exit_status = 0;

	if (!cmd_list)
		return (1);

	// ✅ FIX: Retourner directement la valeur au lieu de if/else
	if (!cmd_list->next)
		return (one_cmd_without_pipe(cmd_list, env));
	
	pids = pid_array(&pipeline, cmd_list);
	if (!pids)
		return (1);
	init_pipeline(&pipeline);

	current_cmd = cmd_list;

	if (loop_pipe(&pipeline, cmd_index, current_cmd, pids, env))
		return (free(pids), 1);

	exit_status = wait_children_pid(&pipeline, pids);
	free(pids);
	return (exit_status);
}

int	one_cmd_without_pipe(t_cmd *cmd_list, t_env *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		exec_simple_cmd(cmd_list, env);
		exit(127);
	}
	else if (pid > 0)
	{
		if (waitpid(pid, &status, 0) == -1)
			return (1);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			int sig;

			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write(2, "Quit (core dumped)\n", 20);
			else if (sig == SIGINT)
				write(2, "\n", 1);
			return (128 + sig);
		}
		return (1);
	}
	perror("fork");
	return (1);
}


// int loop_pipe(t_pipeline *pipeline, int cmd_index, t_cmd *current_cmd, pid_t *pids, t_env *env)
// {
// 	while (current_cmd)
// 	{
// 		pipeline->current_pipe = cmd_index % 2;
// 		if (cmd_index != 0)
// 			pipeline->prev_pipe = (cmd_index - 1) % 2;
// 		else
// 			pipeline->prev_pipe = -1;

// 		if (current_cmd->next)
// 			create_pipe(pipeline);

// 		pids[cmd_index] = fork();

// 		if (pids[cmd_index] == -1)
// 		{
// 			perror("error : fork");
// 			close_all_pipes(pipeline);
// 			while (cmd_index > 0)
// 			{
// 				cmd_index--;
// 				waitpid(pids[cmd_index], NULL, 0);
// 			}
// 			return (1);
// 		}
// 		else if (pids[cmd_index] == 0)
// 		{
// 			child_process(cmd_index, pipeline, current_cmd, env, pids);
// 		}
// 		else
// 		{
// 			parent_process(pipeline, cmd_index);
// 		}
// 		current_cmd = current_cmd->next;
// 		cmd_index++;
// 	}
// 	return (0);
// }

int loop_pipe(t_pipeline *pipeline, int cmd_index, t_cmd *current_cmd, pid_t *pids, t_env *env)
{
	while (current_cmd)
	{
		pipeline->current_pipe = cmd_index % 2;
		if (cmd_index != 0)
			pipeline->prev_pipe = (cmd_index - 1) % 2;
		else
			pipeline->prev_pipe = -1;

		if (current_cmd->next)
			create_pipe(pipeline);

		pids[cmd_index] = fork();

		if (pids[cmd_index] == -1)
		{
			perror("error : fork");
			close_all_pipes(pipeline);
			while (cmd_index > 0)
			{
				cmd_index--;
				waitpid(pids[cmd_index], NULL, 0);
			}
			return (1);
		}
		else if (pids[cmd_index] == 0)
		{
			child_process(cmd_index, pipeline, current_cmd, env, pids);
		}
		else
		{
			// Donner le contrôle du terminal au dernier enfant du pipeline
			if (!current_cmd->next)
				tcsetpgrp(STDIN_FILENO, pids[cmd_index]);
			
			parent_process(pipeline, cmd_index);
		}
		current_cmd = current_cmd->next;
		cmd_index++;
	}
	return (0);
}