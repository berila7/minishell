/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/06/13 16:08:08 by anachat          ###   ########.fr       */
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

int	valid_nflag(char *flag, int *passed_flags)
{
	size_t	i;

	i = 0;
	if (flag[i++] != '-')
	{
		*passed_flags = 1;
		return (0);
	}
	if (!flag[i])
		return (0);
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(char **args)
{
	int	new_line;
	int	passed_flags;
	int	i;

	passed_flags = 0;
	new_line = 1;
	if (count_args(args) > 1)
	{
		i = 1;
		while (args[i])
		{
			if (!passed_flags && valid_nflag(args[i], &passed_flags))
				new_line = 0;
			else
			{
				ft_putstr_fd(args[i], STDOUT_FILENO);
				if (args[i + 1])
					ft_putchar_fd(' ', STDOUT_FILENO);
			}
			i++;
		}
	}
	if (new_line)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
