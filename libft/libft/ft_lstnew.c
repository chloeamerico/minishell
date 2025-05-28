/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:23:08 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:32:03 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Ajoute l’élément ’new’ au début de la liste.
t_list	*ft_lstnew(void *content)
{
	t_list	*v;

	v = malloc(sizeof(t_list));
	if (!v)
		return (NULL);
	v->content = content;
	v->next = NULL;
	return (v);
}
