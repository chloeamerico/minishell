/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:43:23 by camerico          #+#    #+#             */
/*   Updated: 2025/06/09 15:03:42 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


int	main(void)
{
	char	*line;
	char	**split;
	int		i;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break; // CTRL+D
		if (line[0])
			add_history(line);
		split = split_minishell(line);
		if (!split)
		{
			free(line);
			continue;
		}
		i = 0;
		while (split[i])
		{
			printf("Token %d: [%s]\n", i, split[i]);
			free(split[i]);
			i++;
		}
		free(split);
		free(line);
	}
	return (0);
}
