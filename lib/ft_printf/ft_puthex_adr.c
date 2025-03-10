/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex_adr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:29:56 by mberila           #+#    #+#             */
/*   Updated: 2024/11/25 10:35:30 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthex_adr(size_t n, char sp)
{
	int	result;

	result = 0;
	if (n < 16)
	{
		if (sp == 'x')
			result += ft_putchar("0123456789abcdef"[n]);
		else
			result += ft_putchar("0123456789ABCDEF"[n]);
	}
	else
	{
		result += ft_puthex_adr(n / 16, sp);
		result += ft_puthex_adr(n % 16, sp);
	}
	return (result);
}
