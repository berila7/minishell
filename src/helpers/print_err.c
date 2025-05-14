/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:04:18 by anachat           #+#    #+#             */
/*   Updated: 2025/05/06 10:54:57 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_err(char *fmt, void *arg)
{
	int	i;

	if (!fmt)
		return ;
	i = 0;
	while (fmt[i])
	{
		if (fmt[i] == '%' && fmt[i + 1])
		{
			i++;
			if (fmt[i] == 'c')
				ft_putchar_fd(*(char *)arg, 2);
			else if (fmt[i] == 's')
				ft_putstr_fd((char *)arg, 2);
			else if (fmt[i] == 'd')
				ft_putnbr_fd(*(int *)arg, 2);
			else
				ft_putchar_fd(fmt[i], 2);
		}
		else
			ft_putchar_fd(fmt[i], 2);
		i++;
	}
}
