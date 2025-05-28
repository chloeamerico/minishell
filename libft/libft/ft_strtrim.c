/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:13:08 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 15:58:41 by camerico         ###   ########.fr       */
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

// char	*ft_strchr(const char *s, int c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == c)
// 			return ((char *)(s + i));
// 		i++;
// 	}
// 	return (NULL);
// }

// size_t	ft_strlcpy(char * dst, const char * src, size_t siz)
// {
// 	size_t	i;
// 	size_t	sizesrc;

// 	i = 0;
// 	sizesrc = ft_strlen(src);
// 	if (!dst || !src)
// 		return (0);
// 	if (siz != 0)
// 	{
// 		while (src[i] && i < (siz -1))
// 		{
// 			dst[i] = src[i];
// 			i++;
// 		}
// 	}
// 	dst[i] = '\0';
// 	return (sizesrc);
// }

// retourne une copie de s1 sans les char set au deb et la fin de la string.
// utilisation de malloc
char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	char	*s2;

	i = 0;
	j = ft_strlen(s1);
	if (!set || !s1)
		return (NULL);
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	while (j > i && ft_strchr(set, s1[j - 1]))
		j--;
	s2 = malloc(sizeof(char) * (j - i + 1));
	if (!s2)
		return (NULL);
	ft_strlcpy(s2, &s1[i], j - i + 1);
	return (s2);
}

// int	main(int ac, char **av)
// {
// 	(void) ac;
// 	char	*tmp;
// 	tmp = ft_strtrim(av[1], av[2]);
// 	printf("%s", tmp);
// 	free (tmp);
// 	return (0);
// }