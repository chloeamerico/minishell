/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:56:22 by camerico          #+#    #+#             */
/*   Updated: 2025/02/21 20:10:57 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// fonction pour print un unsigned int base 10 (%u)
int	print_unsigned(unsigned int n)
{
	int		i;
	char	c;

	i = 0;
	if (n >= 10)
		i += print_unsigned(n / 10);
	c = (n % 10) + 48;
	i += ft_putchar_printf(c);
	return (i);
}

// int	main(void)
// {
// 	unsigned int	n = 12345;

// 	printf("%u", print_unsigned(n));
// 	return (0);
// }