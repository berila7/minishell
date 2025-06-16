/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/06/16 10:36:18 by anachat          ###   ########.fr       */
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

int	valid_nflag(char *flag, int *valid)
{
	size_t	i;
 
	if (flag[0] != '-')
		return (*valid = 0, 0);
	i = 1;
	if (!flag[i])
		return (*valid = 0, 0);
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (*valid = 0, 0);
		i++;
	}
	return (*valid = 1, 1);
}

int	ft_echo(char **args)
{
	int	new_line;
	int	nflag;
	int	i;

	nflag = 1;
	new_line = 1;
	if (count_args(args) > 1)
	{
		i = 1;
		while (args[i])
		{
			if (nflag && valid_nflag(args[i], &nflag))
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
