/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:56:17 by mberila           #+#    #+#             */
/*   Updated: 2025/06/28 16:55:34 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int		sign;
	long	num;
	long	holder;

	holder = 0;
	while (*str && ((*str >= 9 && *str <= 13) || *str == 32))
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	num = 0;
	while (ft_isdigit(*str))
	{
		num = num * 10 + (*str - '0');
		if (num < holder)
			return (-(sign + 1) / 2);
		holder = num;
		str++;
	}
	return (num * sign);
}
