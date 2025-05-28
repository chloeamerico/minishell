/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:46:44 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:58:30 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// localise la premiere occurence de little dans big, on recherche len charact
// retourne un pointeur vers le premier char de l'occuurence
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (little[j] == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		if (big[i] == little[0])
		{
			while (little[j] && big[i + j] == little[j] && (i + j) < len)
				j++;
			if (little[j] == '\0')
				return ((char *)&big[i]);
		}
		i++;
	}
	return (0);
}

// int	main(int argc, char **argv)
// {
// 	(void) argc;
// 	printf("%s", ft_strnstr(argv[1], argv[2], atoi(argv[3])));
// 	return (0);
// }