/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:58:23 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:34:43 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}*/

// copy src dans dst de taille siz
size_t	ft_strlcpy(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	sizesrc;

	i = 0;
	sizesrc = ft_strlen(src);
	if (!dst || !src)
		return (0);
	if (siz > 0)
	{
		while (src[i] && i < (siz -1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (sizesrc);
}

// int	main(int argc, char **argv)
// {
// 	(void) argc;
// 	printf("%zu", ft_strlcpy(argv[1], argv[2], atoi(argv[3])));
// 	return (0);
// }

/*int main(void)
{
    char    src[] = "i am a freaking legend";
    char    dest[10];
    unsigned int size = 10;
    unsigned int len = ft_strlcpy(dest, src, size);

    printf("length src : %zu\n", ft_strlen(src));
    printf("src : '%s'\n", src);
    printf("dest post copy : '%s'\n", dest);
    printf("length dest : %u\n", len);
}*/