/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:20:54 by camerico          #+#    #+#             */
/*   Updated: 2024/11/27 13:27:37 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// t_list	*ft_lstlast(t_list *lst)
// {
// 	if (!lst)
// 		return (NULL);
// 	while (lst->next != NULL && lst)
// 		lst->next = lst;
// 	return (lst);
// }

// Ajoute l’élément ’new’ à la fin de la liste.
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ft_lstlast(*lst)->next = new;
}
