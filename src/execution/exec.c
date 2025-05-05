/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:36:35 by anachat           #+#    #+#             */
/*   Updated: 2025/05/05 15:32:28 by anachat          ###   ########.fr       */
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
	int	exit_st;

	if (count_cmd(data->cmds) == 1)
	{
		if (is_builtin(data->cmds))
		{
			data->og_fd[0] = dup(STDIN_FILENO);
			data->og_fd[1] = dup(STDOUT_FILENO);
			if (handle_redirections(data->cmds, data))
				return (1);
			exec_builtin(data->cmds, data, 1);
			dup2_og(data);
			check_fds_in_child("Parent BuiltIn:");
			if (equal(data->cmds->args[0], "exit"))
			{
				exit_st = data->exit_status;
				free_data(data);
				exit(exit_st);
			}
		}
		else
			data->exit_status = exec_single_cmd(data);
	}
	else
		data->exit_status = exec_multiple_cmd(data);
	return (0);
}
