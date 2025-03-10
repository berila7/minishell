/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:03:15 by mberila           #+#    #+#             */
/*   Updated: 2024/11/25 15:35:59 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_check(va_list args, char c)
{
	int	result;

	result = 0;
	if (c == 'd' || c == 'i')
		result = ft_putnbr(va_arg(args, int), 1);
	else if (c == 'c')
		result = ft_putchar(va_arg(args, int));
	else if (c == 's')
		result = ft_putstr(va_arg(args, char *));
	else if (c == 'x' || c == 'X')
		result = ft_puthex_adr(va_arg(args, unsigned int), c);
	else if (c == 'p')
	{
		result = ft_putstr("0x");
		result += ft_puthex_adr(va_arg(args, size_t), 'x');
	}
	else if (c == 'u')
		result = ft_putnbr(va_arg(args, unsigned int), 0);
	else if (c == '%')
		return (ft_putchar('%'));
	else
		return (ft_putchar(c));
	return (result);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	va_list	args;
	int		result;

	if (write(1, "", 0) == -1)
		return (-1);
	va_start(args, str);
	result = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			if (str[i] == '\0')
				break ;
			result += ft_check(args, str[i]);
		}
		else
			result += ft_putchar(str[i]);
		i++;
	}
	va_end(args);
	return (result);
}
