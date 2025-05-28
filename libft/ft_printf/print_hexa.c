/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 18:50:51 by camerico          #+#    #+#             */
/*   Updated: 2025/02/21 18:37:41 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// fonction pour print un nombre hexadecimal (%x ou %X)
int	print_hexa(unsigned long n, const char format)
{
	int		len;
	char	*hexa;

	len = 0;
	if (format == 'x')
		hexa = "0123456789abcdef";
	if (format == 'X')
		hexa = "0123456789ABCDEF";
	if (n >= 16)
		len += print_hexa(n / 16, format);
	len += ft_putchar_printf(hexa[n % 16]);
	return (len);
}
