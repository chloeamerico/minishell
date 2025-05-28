/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:42:20 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:32:15 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// compare les n premiers octets de *s1 et *s2
// renvoie un entier inferieur, egal ou sup a 0 si s1 diff de s2
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

// int	main(int argc, char **argv)
// {
// 	(void) argc;
// 	int	ptr1;
// 	int	ptr2;

// 	ptr1 = ft_memcmp(argv[1], argv[2], atoi(argv[3]));
// 	ptr2 = memcmp(argv[1], argv[2], atoi(argv[3]));
// 	printf("your %d \n", ptr1);
// 	printf("normal %d", ptr2);
// 	return (0);
// }