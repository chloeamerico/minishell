/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:33:36 by camerico          #+#    #+#             */
/*   Updated: 2025/02/21 18:37:21 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_formats(va_list args, const char format)
{
	int	print_length;

	print_length = 0;
	if (format == 'c')
		print_length += ft_putchar_printf(va_arg(args, int));
	else if (format == 's')
		print_length += ft_putstr(va_arg(args, char *));
	else if (format == 'p')
		print_length += print_pointeur(va_arg(args, void *));
	else if (format == 'd' || format == 'i')
		print_length += print_integer(va_arg(args, int));
	else if (format == 'u')
		print_length += print_unsigned(va_arg(args, unsigned int));
	else if (format == 'x' || format == 'X')
		print_length += print_hexa(va_arg(args, unsigned int), format);
	else if (format == '%')
		print_length += ft_putchar_printf('%');
	return (print_length);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	va_list	args;
	int		print_length;

	i = 0;
	print_length = 0;
	va_start(args, str);
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '%')
		{
			print_length += ft_formats(args, str[i + 1]);
			i++;
		}
		else
			print_length += ft_putchar_printf(str[i]);
		i++;
	}
	va_end(args);
	return (print_length);
}
