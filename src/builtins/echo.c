/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/04/25 13:17:21 by anachat          ###   ########.fr       */
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
			ft_putstr_fd(args[i], STDOUT_FILENO);
			i++;
			if (args[i])
				ft_putchar_fd(' ', STDOUT_FILENO);
		}
	}
	if (new_line)
		ft_putchar_fd('\n', STDOUT_FILENO);

}
