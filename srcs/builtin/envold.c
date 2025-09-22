/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envold.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:07:54 by camerico          #+#    #+#             */
/*   Updated: 2025/09/22 17:22:30 by lleichtn         ###   ########.fr       */
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
