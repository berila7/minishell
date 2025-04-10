/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/04/10 18:03:07 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	echo(char **args)
{
	int	i;
	int	new_line;

	new_line = 1;
	if (count_args(args) > 1)
	{
		i = 0;
		while (args[++i] && ft_strncmp("-n", args[i], 3) == 0)
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


// int main(int argc, char const *argv[])
// {
// 	// simulate passing an array of strings like {"echo", "Hello", "World"}
// 	char *args[] = {"echo", "Hello", "-n", "World", NULL};  // NULL-terminated array
// 	echo(args);
// 	return 0;
// }
