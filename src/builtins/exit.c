/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/05/18 12:25:20 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_long(const char *str)
{
	int		i;
	int		s;
	long	nb;

	s = 1;
	i = 0;
	nb = 0;
	while (((str[i] >= 9 && str[i] <= 13) || str[i] == ' ' || str[i] == '	'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			s = -1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i] && ft_isdigit(str[i]))
	{
		if (nb > 922337203685477580 || ((nb == 922337203685477580)
				&& ((s == 1 && str[i] > '7') || (s == -1 && str[i] > '8'))))
			return (0);
		nb = nb * 10 + (str[i++] - '0');
	}
	return (1);
}

int	ft_exit(char **args, t_data *data)
{
	int		status;
	long	nb;

	status = 0;
	(void)data;
	printf("exit\n");
	if (args[1] && !is_long(args[1]))
	{
		print_err("exit: %s: numeric argument required\n", args[1]);
		status = 255;
	}
	else if (args[1])
	{
		nb = ft_atol(args[1]);
		if (nb < 0)
			status = 256 + nb;
		else
			status = (nb % 256);
	}
	else if (args[0] && args[1] && args[2])
	{
		print_err("exit: too many arguments\n", NULL);
		status = 1;
	}
	exit_status(status, 1);
	return (0);
}
