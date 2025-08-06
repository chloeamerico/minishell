/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 16:05:29 by chloeameric       #+#    #+#             */
/*   Updated: 2025/08/06 18:58:39 by camerico         ###   ########.fr       */
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
 


 
//fonction qui appeler toutes les autres, debut de l'exec
void	exec_pipeline(t_cmd *cmd_list, t_env *env)
{
	int	nb_pipe;
	char	**envp;
	t_cmd	*cmd = cmd_list;
	t_pipeline	pipeline;		//fait pour stocker les données liées aux pipes, si rien d'autres que nb_cmd, retirer la structure et mettre direct la variable
	
	init_struct_pipe(&pipeline, cmd);
	envp = env_to_array(env);
	nb_pipe = pipeline.nb_cmd - 1;
	int	pipe_fd[nb_pipe][2];		//pour les extremites de lecture et d'ecriture
	if(pipeline.nb_cmd == 1)
		return(exec_simple_cmd());			//fonction a faire ! 
	else
		pipe_loop(cmd, pipe_fd, &pipeline, envp);

	

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
	pipeline->nb_pipe = i - 1;
}


//fonction qui va boucler pour appliquer a tous les t_cmd
//dans chaque appel, on va creer un pipe, forker, rediriger les entrées/sorties avec dup2, et executer avec execve.
int	pipe_loop(t_cmd *cmd, int pipe_fd[][2], t_pipeline *pipeline, char **envp)
{
	pid_t	pid;
	int	i;

	i = 0;
	while(cmd)
	{
		if(cmd->next)		//si on est pas à la derniere commande, on cree un pipe
		{
			if(pipe(pipe_fd[i]) == -1)
				perror("error pipe");
		}
		pid = fork();
		if (pid == -1)
			perror("error fork");
		
		if (pid == 0)		//on est dans le process enfant
			child_process(cmd, pipe_fd, &pipeline, envp, i);
		cmd = cmd->next;
		i++;
	}
}

void	child_process(t_cmd *cmd, int pipe_fd[][2], t_pipeline *pipeline, char **envp, int i)
{
	if (i == 0)		//si on est dans la 1ere commande
		dup2(pipe_fd[i][1], STDOUT_FILENO); 		//on redirige la sortie de la cmd vers le pipe
	else if (i == pipeline->nb_cmd - 1)		//si on est a la derniere commande 
		dup2(pipe_fd[i - 1][0], STDIN_FILENO);
	else		//cmd du milieu
	{
		dup2(pipe_fd[i - 1][0] ,STDIN_FILENO);
		dup2(pipe_fd[i][1], STDOUT_FILENO);
	}
	close_all_pipes(pipeline, pipe_fd);			//a faire
	exec_simple_cmd();			//a faire		builtins ou execve
	
	exit(1);	//si il y a eu un pb
}


void	close_all_pipes(t_pipeline *pipeline, int pipe_fd[][2])
{
	int	i;

	i = 0;
	while(i < pipeline->nb_pipe)
	{
		close(pipe_fd[i][1]);
		close(pipe_fd[i][0]);
		i++;
	}
}


