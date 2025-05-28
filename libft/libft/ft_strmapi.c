/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:21:14 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:44:45 by camerico         ###   ########.fr       */
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

// applique la fonction f a tous les char de la string s
// utilisation de malloc
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*final;

	i = 0;
	final = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!final)
		return (NULL);
	while (s[i])
	{
		final[i] = (*f)(i, s[i]);
		i++;
	}
	final[i] = '\0';
	return (final);
}

// char	ft_toupper2(unsigned int in, char c)
// {
// 	(void) in;
// 	if (c >= 'a' && c <= 'z')
// 		c -= 32;
// 	return (c);
// }

// int	main(void)
// {
// 	char	s[] = "hello";
// 	char	*tmp;

// 	tmp = ft_strmapi(s, ft_toupper2);
// 	printf("%s", tmp);
// 	free(tmp);
// 	return (0);
// }