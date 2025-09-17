/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:07:54 by camerico          #+#    #+#             */
/*   Updated: 2025/09/17 14:20:59 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//fonction pour imprimer la var d'env si on fait "env"
int	print_env(char **env)
{
	int	i;

	i = 0;
	while(env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
