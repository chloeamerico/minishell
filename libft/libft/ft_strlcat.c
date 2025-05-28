/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:36:45 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:33:34 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <bsd/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// concatene 2 strings, src a la fin de dst (siz est la taille totale de dst)
size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	j;
	size_t	dest_len;
	size_t	src_len;

	dest_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dest_len >= siz)
		return (siz + src_len);
	i = 0;
	j = 0;
	while (dst[i])
		i++;
	while (src[j] && j < siz - dest_len - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	if (siz < dest_len)
		return (siz + ft_strlen(src));
	return (dest_len + ft_strlen(src));
}

// int	main(int argc, char **argv)
// {
// 	(void) argc;
// 	char str[25] = "hey";
// 	char str2[25] = "hey";

// 	printf("%zu\n", ft_strlcat(str, argv[1], 25));
// 	printf("%s\n", str);
// 	printf("%zu\n", strlcat(str2, argv[1], 25));
// 	printf("%s\n", str2);
// 	return (0);
// }
