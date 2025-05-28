/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:35:49 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:33:25 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// size_t	ft_strlen(const char *s)
// {
// 	size_t	i;

// 	i = 0;
// 	while (s[i])
// 		i++;
// 	return (i);
// }

// retourne un pointeur vers une nouvelle string copie de *s
// utilisation de malloc, attention a free
char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*dest;

	i = 0;
	dest = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

// int	main(int argc, char **argv)
// {
// 	(void) argc;
// 	printf("%s", ft_strdup(argv[1]));
// 	free(ft_strdup(argv[1]));
// 	return (0);
// }