/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:26:32 by camerico          #+#    #+#             */
/*   Updated: 2025/02/10 15:42:22 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// recherche la premiere occurence du char c dans *s
// retourne le reste de la string a partir du premier char c trouve
// char	*ft_strchr(const char *s, int c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i] && s[i] != (char)c)
// 		i++;
// 	if (s[i] == (char)c)
// 		return ((char *)(s + i));
// 	else
// 		return (NULL);
// }

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != (char)c)
		i++;
	if (s[i] == (char)c)
		return ((char *)(s + i));
	else
		return (NULL);
}

// int	main(int argc, char **argv)
// {
// 	(void) argc;
// 	printf("%s", ft_strchr(argv[1], argv[2][0]));
// 	return (0);
// }