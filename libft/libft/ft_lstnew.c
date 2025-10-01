/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:23:08 by camerico          #+#    #+#             */
/*   Updated: 2025/10/01 11:49:27 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Ajoute l’élément ’new’ au début de la liste.
t_list	*ft_lstnew(void *content)
{
	t_list	*v;

	v = malloc (sizeof(t_list));
	if (!v)
		return (NULL);
	v->content = content;
	v->next = NULL;
	return (v);
}
