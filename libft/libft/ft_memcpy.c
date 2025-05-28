/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:24:10 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:32:18 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// copie n octets de *src vers *dest sans se chevaucher
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (!dest && !src)
		return (0);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((const unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

// void	*ft_memcpy(void *dest, const void *src, size_t n)
// {
// 	char	*d;
// 	char	*s;
// 	int	i;

// 	d = (unsigned char *) dest;
// 	s = (const unsigned char *) src;
// 	i = 0;
// 	if (d)

// int	main(int argc, char **argv)
// {
// 	(void) argc;
// 	ft_memcpy(argv[1], argv[2], atoi(argv[3]));
// 	return (0);
// }