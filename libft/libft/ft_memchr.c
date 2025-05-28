/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:57:50 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:32:09 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// parcours les n premiers octets de la memoire pointee par *s + recherche c
// revoie un pointeur sur l'octet correspondant a c
void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (i < n)
	{
		if (str[i] == (char)c)
			return ((void *)str + i);
		i++;
	}
	return (NULL);
}

// int	main(int argc, char **argv)
// {
// 	(void) argc;
// 	void	*ptr;
// 	void	*ptr2;
// 	ptr = ft_memchr(argv[1], argv[2][0], atoi(argv[3]));
// 	ptr2 = memchr(argv[1], argv[2][0], atoi(argv[3]));
// 	printf("your %s \n%p -- %s\n", argv[1], (char *)ptr, (char *)ptr);
// 	printf("normal %s \n%p -- %s\n", argv[1], (char *)ptr2, (char *)ptr2);
// 	return (0);
// }
