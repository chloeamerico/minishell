/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laurentleichtnam <laurentleichtnam@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:22:42 by camerico          #+#    #+#             */
/*   Updated: 2025/08/08 15:33:44 by laurentleic      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//lliste des builtins a prendre en compte dans minishell :
// echo, cd, pwd, export, unset, env, exit

#include "minishell.h"

//va retourner 1 si au moins un des strcmp est vrai , sinon retourne 0 si aucun match
//permet de savoir si c'est un builtin
static int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

//permet d'exectuer la bonne action selon la commande
int	exectute_builtin


// A finir 