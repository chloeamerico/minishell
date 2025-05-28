/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:45:53 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 15:59:14 by camerico         ###   ########.fr       */
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

// alloue de la memoire et retourne une nouvelle chaine qui a concatene s1+s2
// utilisation de malloc
char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	int		k;
	char	*s3;

	i = 0;
	j = 0;
	k = 0;
	s3 = malloc(sizeof(char) * ((ft_strlen(s1) + ft_strlen(s2)) + 1));
	if (!s3)
		return (NULL);
	while (s1[i])
	{
		s3[k] = s1[i];
		i++;
		k++;
	}
	while (s2[j])
	{
		s3[k] = s2[j];
		k++;
		j++;
	}
	s3[k] = '\0';
	return (s3);
}

// int	main(int argc, char **argv)
// {
// 	(void) argc;
// 	char *tmp;
// 	tmp = ft_strjoin(argv[1], argv[2]);
// 	printf("%s", tmp);
// 	free (tmp);
// 	return (0);
// }