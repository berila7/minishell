/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 10:47:11 by mberila           #+#    #+#             */
/*   Updated: 2024/11/24 10:20:05 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr(int n, int is_signed)
{
	unsigned int	nb;
	int				result;

	result = 0;
	nb = n;
	if (n < 0 && is_signed == 1)
	{
		result += ft_putchar('-');
		nb = -n;
	}
	if (nb < 10)
		result += ft_putchar(nb + '0');
	else
	{
		result += ft_putnbr(nb / 10, is_signed);
		result += ft_putnbr(nb % 10, is_signed);
	}
	return (result);
}
