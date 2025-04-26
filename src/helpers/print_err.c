/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:04:18 by anachat           #+#    #+#             */
/*   Updated: 2025/04/26 12:12:10 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_err(char *msg, char *var)
{
	size_t	i;

	i = 0;
	while (msg && msg[i])
	{
		if (msg[i] == ':')
		{
			if (!var)
				ft_putstr_fd("(null)", 2);
			else
				ft_putstr_fd(var, 2);
		}
		ft_putchar_fd(msg[i], 2);
		i++;
	}
}
