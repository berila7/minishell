/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:36:35 by anachat           #+#    #+#             */
/*   Updated: 2025/04/26 11:47:55 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_wait(pid_t last_pid, int default_st)
{
	int		status;
	int		exit_st;
	pid_t	pid;

	exit_st = default_st;
	pid = wait(&status);
	while (pid != -1)
	{
		if (pid == last_pid)
			exit_st = WEXITSTATUS(status);
		pid = wait(&status);
	}
	return (exit_st);
}

int	exec(t_data *data)
{
	if (count_cmd(data->cmds) == 1)
	{
		if (is_builtin(data->cmds))
			data->exit_status = exec_builtin(data->cmds, data, 1);
		else
			data->exit_status = exec_single_cmd(data);
	}
	else
		data->exit_status = exec_multiple_cmd(data);
	return (0);
}
