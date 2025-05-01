/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/05/01 15:30:38 by anachat          ###   ########.fr       */
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
	printf("exit\n");
	if (args[1] && !is_long(args[1]))
	{
		printf("exit: %s: numeric argument required\n", args[1]);
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
		printf("exit: too many arguments\n");
		status = 1;	
	}
	free_data(data);
	ft_dup2(data->og_fd[0], STDIN_FILENO);
	ft_dup2(data->og_fd[1], STDOUT_FILENO);
	exit(status);
	return (0);
}

// int main(int ac, char const *av[])
// {
// 	// simulate passing an array of strings like {"exit", "2222", NULL}
// 	char *args[ac + 1];  // Create an array with space for all arguments (including NULL)

// 	// If there are arguments, set the first one to "exit" and others to the av[] values
// 	if (ac > 1) {
// 		args[0] = "exit";  // The first element is always "exit"
// 		for (int i = 1; i < ac; i++) {
// 			args[i] = (char *)av[i];  // Copy other arguments from av[]
// 		}
// 	}
	
// 	args[ac] = NULL;  // Ensure the array is NULL-terminated for the ft_exit function
// 	// mini_exit(args);
// 	int res = ft_exit(args);
// 	printf("\n\n\n---------------------------------");
// 	printf("\n|\t[exit] returns: %d\t|", res);
// 	printf("\n---------------------------------\n\n");
// 	return 0;
// }
