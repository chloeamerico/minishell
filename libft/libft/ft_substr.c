/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:45:35 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 12:40:26 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	size_t	i;
// 	size_t	j;
// 	char	*s2;

// 	i = 0;
// 	j = 0;
// 	s2 = malloc(sizeof(char) * (len + 1));
// 	if (!s2)
// 		return (NULL);
// 	while (s[i])
// 	{
// 		if (i >= start && j < len)
// 			s2[j] = s[i];
// 		j++;
// 		i++;
// 	}
// 	s2[j] = '\0';
// 	return (s2);
// }

// retourne une nouvelle string issue de s qui commence a start et
// de taille max len.  s2 est la nouvelle chaine
// utilisation de *malloc*
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*s2;

	i = ft_strlen(s);
	j = 0;
	if (start >= i)
		return (ft_strdup(""));
	if (len > i - start)
		len = i - start;
	s2 = malloc(sizeof(char) * len + 1);
	if (!s2)
		return (NULL);
	while (s[start] && j < len)
	{
		s2[j] = s[start];
		j++;
		start++;
	}
	s2[j] = '\0';
	return (s2);
}

// int	main(int argc, char **argv)
// {
// 	char	*tmp;
// 	(void) argc;
// 	tmp = ft_substr(argv[1], atoi(argv[2]), atoi(argv[3]));
// 	printf("%s", tmp);
// 	free(tmp);
// 	return (0);
// }