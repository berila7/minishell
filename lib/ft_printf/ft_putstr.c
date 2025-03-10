/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 10:56:24 by mberila           #+#    #+#             */
/*   Updated: 2024/11/23 10:30:56 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putstr(char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return ((write(1, "(null)", 6)));
	while (str[len])
	{
		write(1, &str[len++], 1);
	}
	return (len);
}
