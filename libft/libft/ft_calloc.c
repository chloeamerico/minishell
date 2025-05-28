/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:34:43 by camerico          #+#    #+#             */
/*   Updated: 2024/11/28 18:13:21 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "libft.h"

// alloue de la memoire pour nmemb elemts de taille size
// renvoie un pointeur vers le tableau/l'espace reserve
// utilisation de malloc
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total;

	if (nmemb == 0 || size == 0)
		return ((void *)malloc(1));
	total = nmemb * size;
	if (total / nmemb != size && total / size != nmemb)
		return (NULL);
	ptr = (void *)malloc(total);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total);
	return (ptr);
}
