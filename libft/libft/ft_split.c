/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:36:43 by camerico          #+#    #+#             */
/*   Updated: 2025/03/18 16:49:08 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// char	*ft_strchr(const char *s, int c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i] && s[i] != c)
// 		i++;
// 	if (s[i] == c)
// 		return ((char *)(s + i));
// 	else
// 		return (NULL);
// }

// size_t	ft_strlen(const char *s)
// {
// 	size_t	i;

// 	i = 0;
// 	while (s[i])
// 		i++;
// 	return (i);
// }

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	size_t	i;
// 	size_t	j;
// 	char	*s2;

// 	i = 0;
// 	j = 0;
// 	s2 = malloc((len + 1) * sizeof(char));
// 	if (!s2)
// 		return (NULL);
// 	while (s[i])
// 	{
// 		if (i >= start && j < len)
// 		{
// 			s2[j] = s[i];
// 			j++;
// 		}
// 		i++;
// 	}
// 	s2[j] = '\0';
// 	return (s2);
// }

static size_t	ft_countword(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

void	ft_freeall(char **tab, int nb)
{
	while (nb--)
		free(tab[nb]);
	free(tab);
}

int	split2(char const *s, char **lst, char c)
{
	int		i;
	size_t	word_len;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			if (ft_strchr(s, c))
				word_len = ft_strchr(s, c) - s;
			else
				word_len = ft_strlen(s);
			lst[i] = ft_substr(s, 0, word_len);
			if (!lst[i])
				return (ft_freeall(lst, i), 1);
			i++;
			s += word_len;
		}
	}
	lst[i] = NULL;
	return (0);
}

// recherche la premiere occurence du char c dans *s
// retourne un tableau de stings obtenues chacune par le delimiteur c
// doit etre termine par NULL
//utilisation de malloc
char	**ft_split(char const *s, char c)
{
	char	**lst;
	int		num_word;

	num_word = ft_countword(s, c);
	lst = (char **)malloc(sizeof(char *) * (num_word + 1));
	if (!lst)
		return (NULL);
	if (split2(s, lst, c) == 1)
		return (NULL);
	return (lst);
}

// int	main(int argc, char **argv)
// {
// 	char	**tmp;
// 	int		i;

// 	if (argc != 3)
// 	{
// 		printf("Usage: %s <string_to_split> <delimiter>\n", argv[0]);
// 		return (1);
// 	}
// 	tmp = ft_split(argv[1], argv[2][0]);
// 	if (!tmp)
// 	{
// 		printf("Error: Memory allocation failed.\n");
// 		return (1);
// 	}
// 	i = 0;
// 	while (tmp[i])
// 	{
// 		printf("tmp[%d] = '%s'\n", i, tmp[i]);
// 		free(tmp[i]);
// 		i++;
// 	}
// 	free(tmp);
// 	return (0);
// 	char *ft_substr(char const *s, unsigned int start, size_t len);
// }
