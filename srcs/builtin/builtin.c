/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laurentleichtnam <laurentleichtnam@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:22:42 by camerico          #+#    #+#             */
/*   Updated: 2025/08/19 17:02:25 by laurentleic      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//lliste des builtins a prendre en compte dans minishell :
// echo, cd, pwd, export, unset, env, exit

#include "minishell.h"

//va retourner 1 si au moins un des strcmp est vrai , sinon retourne 0 si aucun match
//permet de savoir si c'est un builtin
// static int	is_builtin(char *cmd)
// {
// 	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
// 		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
// 		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
// 		|| !ft_strcmp(cmd, "exit"));
// }

static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	// if (!ft_strcmp(cmd, "export"))
	// 	return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	// if (!ft_strcmp(cmd, "env"))
	// 	return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	execute_builtin(char **args, char ***envp)
{
	if (!args || !args[0])
		return (1);
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(args));
	if (!ft_strcmp(args[0], "cd"))
		return (ft_cd(args, envp));
	if (!ft_strcmp(args[0], "pwd"))
		return (ft_pwd());
	// if (!ft_strcmp(args[0], "export"))
	// 	return (ft_export(args, envp));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(args, envp));
	// if (!ft_strcmp(args[0], "env"))
	// 	return (ft_env(*envp));
	if (!ft_strcmp(args[0], "exit"))
		return (ft_exit(args));
	return (1);
}

int	try_run_builtin(char **args, char ***envp, int *status)
{
	int	code;

	if (!args || !args[0] || !is_builtin(args[0]))
		return (0);
	code = execute_builtin(args, envp);
	if (status)
		*status = code;
	return (1);
}