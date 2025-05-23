/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:31:57 by mberila           #+#    #+#             */
/*   Updated: 2025/05/23 11:31:49 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	digits_count(int n)
{
	unsigned int	temp;
	int				len;

	len = 0;
	temp = n;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		temp = -n;
		len++;
	}
	while (temp)
	{
		temp /= 10;
		len++;
	}
	return (len);
}

char	*gc_itoa(t_gcnode **gc, int n)
{
	char			*result;
	int				len;
	unsigned int	num;

	len = digits_count(n);
	result = gc_malloc(gc, (len + 1) * sizeof(char));
	num = n;
	if (n < 0)
		num = -n;
	while (len >= 0 && num != 0)
	{
		len--;
		result[len] = '0' + (num % 10);
		num /= 10;
	}
	if (n < 0)
		result[0] = '-';
	else if (n == 0)
		result[0] = '0';
	return (result);
}
