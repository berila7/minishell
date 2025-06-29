/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/06/28 16:21:49 by anachat          ###   ########.fr       */
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
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			s = -1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]) || nb > LMAX_DIV10 || ((nb == LMAX_DIV10)
				&& ((s == 1 && str[i] > '7') || (s == -1 && str[i] > '8'))))
			return (0);
		nb = nb * 10 + (str[i++] - '0');
	}
	return (1);
}

static char	*trim_end(char *str)
{
	int	i;

	if (!str)
		return (NULL);
	i = strlen(str);
	while (i > 0 && ((str[i - 1] >= 9
				&& str[i - 1] <= 13) || str[i - 1] == ' '))
		i--;
	str[i] = '\0';
	return (str);
}

int	ft_exit(char **args, int exit_flag)
{
	int		status;
	long	nb;

	status = exit_status(0, 0);
	if (exit_flag)
		print_err("exit\n", NULL);
	if (args[1] && !is_long(trim_end(args[1])))
	{
		print_err("exit: %s: numeric argument required\n", args[1]);
		status = 2;
	}
	else if (count_args(args) > 2)
	{
		print_err("%s too many arguments\n", "exit:");
		status = 999;
	}
	else if (args[1])
	{
		nb = ft_atol(args[1]);
		if (nb < 0)
			status = 256 + nb;
		else
			status = (nb % 256);
	}
	return (status);
}
