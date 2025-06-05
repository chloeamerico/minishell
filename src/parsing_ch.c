/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_ch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:17:43 by camerico          #+#    #+#             */
/*   Updated: 2025/06/05 16:45:54 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**word_to_array(char **array, char *word)
{
	int	i;
	char **new_array;
	
	i = 0;
	while(array && array[i])
		i++;
	new_array = malloc(sizeof(char *) * (i + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	while(array && array[i])
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = word;
	new_array[i + 1] = NULL;
	free(array);
	return (new_array);
}