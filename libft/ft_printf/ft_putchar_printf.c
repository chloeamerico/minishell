/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:45:25 by camerico          #+#    #+#             */
/*   Updated: 2025/02/21 18:28:40 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// fonction pour print un charactere unique (%c)
int	ft_putchar_printf(char c)
{
	write(1, &c, 1);
	return (1);
}
