/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/05/23 16:01:32 by ayoub            ###   ########.fr       */
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

int	valid_nflag(char *flag)
{
	size_t	i;

	i = 0;
	if (flag[i++] != '-' || !flag[i])
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
	int	i;

	new_line = 1;
	if (count_args(args) > 1)
	{
		i = 1;
		while (args[i])
		{
			if (valid_nflag(args[i]))
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
