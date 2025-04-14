/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:36:35 by anachat           #+#    #+#             */
/*   Updated: 2025/04/14 14:47:27 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec(t_data *data)
{
	// int	main(int ac, char **av, char **env)
	// pid_t	last_pid;
	// int		exit_st;
	// int		fd[7];
	// int		i;

	if (count_cmd(data->cmds) == 1 && is_builtin(data->cmds))
		return (exec_builtin(data->cmds, data), 0);
	

	
	// fd[3] = dup(0);
	// fd[4] = dup(1);
	// while (i < ac - 2)
	// 	parent1(fd, av, i++, env);
	// fd[6] = i;
	// exit_st = parent2(fd, av, env, &last_pid);
	// ft_dup2(fd[3], 0);
	// ft_dup2(fd[4], 1);
	// return (ft_wait(last_pid, exit_st));
	return (0);
}