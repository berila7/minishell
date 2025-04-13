/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/04/13 16:36:30 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	ft_echo(char **args)
{
	int	i;
	int	new_line;

	new_line = 1;
	if (count_args(args) > 1)
	{
		i = 0;
		while (args[++i] && ft_strcmp("-n", args[i]) == 0)
			new_line = 0;
		while (args[i])
		{
			ft_putstr_fd(args[i], 1);
			i++;
			if (args[i])
				ft_putchar_fd(' ', 1);
		}
	}
	if (new_line)
		ft_putchar_fd('\n', 1);
}
