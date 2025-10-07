/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:11:53 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 15:13:38 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fonction qui va prendre un mot et l'ajouter au tableau array
char	**word_to_array(char **array, char *word)
{
	int		i;
	char	**new_array;

	i = 0;
	while (array && array[i])
		i++;
	new_array = malloc (sizeof(char *) * (i + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	while (array && array[i])
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = word;
	new_array[i + 1] = NULL;
	free(array);
	return (new_array);
}

int	check_close_quotes(char *line)
{
	int		i;
	char	quotes;

	i = 0;
	while (line[i])
	{
		if (line[i] == 39 || line[i] == 34)
		{
			quotes = line[i];
			i++;
			while (line[i] && line[i] != quotes)
				i++;
			if (!line[i])
			{
				ft_putendl_fd("Syntax error: unclosed quote", 2);
				return (1);
			}
			i++;
		}
		else
			i++;
	}
	return (0);
}
