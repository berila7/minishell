/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atol.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:21:16 by anachat           #+#    #+#             */
/*   Updated: 2025/05/16 20:09:37 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*ft_strjoin_char_free(char *str, char c)
{
	char	*result;

	result = ft_strjoin_char(str, c);
	free(str);
	return (result);
}
