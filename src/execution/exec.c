/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:36:35 by anachat           #+#    #+#             */
/*   Updated: 2025/05/14 18:27:40 by anachat          ###   ########.fr       */
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
		dup_og(data);
		if (is_builtin(data->cmds))
		{
			if (handle_redirections(data->cmds, data))
				return (data->exit_status = 1, 1);
			exec_builtin(data->cmds, data);
		}
		else
		{
			data->exit_status = exec_single_cmd(data);
			dup2_og(data);
		}
	}
	else
		data->exit_status = exec_multiple_cmd(data);
	return (0);
}
