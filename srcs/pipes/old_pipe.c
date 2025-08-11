/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 16:05:29 by chloeameric       #+#    #+#             */
/*   Updated: 2025/08/11 17:12:07 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// le pipe a deux extremites :
// pipefd[0] -> extremite de lecture du pipe
// pipefd[1] -> extremite d'écriture du pipe

#include "minishell.h"


//fonction qui appeler toutes les autres, debut de l'exec
void	exec_pipeline(t_cmd *cmd_list, t_env *env)
{
	int	nb_pipe;
	char	**envp;
	t_cmd	*cmd = cmd_list;
	t_pipeline	pipeline;		//fait pour stocker les données liées aux pipes, si rien d'autres que nb_cmd, retirer la structure et mettre direct la variable
	
	init_struct_pipe(&pipeline, cmd);
	envp = env_to_array(env);
	pipeline.envp = envp;
	nb_pipe = pipeline.nb_cmd - 1;
	int	pipe_fd[nb_pipe][2];		//pour les extremites de lecture et d'ecriture
	if(pipeline.nb_cmd == 1)
		return(exec_simple_cmd(&pipeline, cmd, pipe_fd));			//fonction a faire ! 
	else
		pipe_loop(cmd, pipe_fd, &pipeline);

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
int	pipe_loop(t_cmd *cmd, int pipe_fd[][2], t_pipeline *pipeline)
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
			child_process(cmd, pipe_fd, &pipeline, i);
		cmd = cmd->next;
		i++;
	}
}

void	child_process(t_cmd *cmd, int pipe_fd[][2], t_pipeline *pipeline, int i)
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
	close_all_pipes(pipeline, pipe_fd);
	exec_simple_cmd();			//a faire		builtins ou execve
	
	exit(1);	//si il y a eu un pb
}



int	exec_simple_cmd(t_pipeline *pipeline, t_cmd *cmd,  int pipe_fd[][2])
{
	char	*cmd_path;
	char	**envp;
	char	**cmd_arg;

	envp = pipeline->envp;
	
	if(!cmd)
	{
		close_all_pipes(pipeline, pipe_fd);
		exit(1);
	}
	cmd_arg = ft_split(cmd->args, ' ');
	if (!cmd_arg || !cmd_arg[0])
	{
		ft_printf("Error : invalid command");
		close_all_pipes(pipeline, pipe_fd);
		free_tab(cmd_arg);
		exit (1);
	}
	if (is_builtins(cmd->args[0]))		//si c'est un builtin
	{
		exec_builtins();				//on fait les execute comme des builtins
		exit(0);
	}
	cmd_path = find_cmd_path(cmd_arg[0], envp);
	if (!cmd_path)
		cmd_not_found(cmd_arg, cmd_arg[0]);
	execve(cmd_path, cmd_arg, envp);
	perror("execve failed");
	free_tab(cmd_arg);
	free(cmd_path);
	close_all_pipes(pipeline, pipe_fd);
	exit(1);
}

char	*find_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*tmp;
	char	*full_path;

	if (access(cmd, X_OK) == 0)
		return(cmd);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL); // faire attention si on retire le chemin
	paths = (ft_split(envp[i] + 5, ':'));
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		if (access(full_path, X_OK) == 0)
			return (free(tmp), free(paths), full_path);
		free(full_path);
		free(tmp);
		i++;
	}
	free_tab(paths);
	return (NULL);
}
