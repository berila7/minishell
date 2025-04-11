/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atol.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:21:16 by anachat           #+#    #+#             */
/*   Updated: 2025/04/11 16:18:46 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_spaces_and_sign(const char *str, int *i, int *sign)
{
	*sign = 1;
	while (str[*i] && ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32))
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign = -1;
		(*i)++;
	}
	return (1);
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	i = 0;
	result = 0;
	handle_spaces_and_sign(str, &i, &sign);
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
