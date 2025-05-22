/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:36:35 by anachat           #+#    #+#             */
/*   Updated: 2025/05/22 20:54:57 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <sys/wait.h>
#include <unistd.h>

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
		{
			if (WIFEXITED(status))
				exit_st = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_st = 128 + WTERMSIG(status);
		}
		pid = wait(&status);
	}
	printf("exit_st: %d\n", exit_st);
	return (exit_st);
}





// else if (WIFSIGNALED(status))
// 	exit_status(1, 128 + WTERMSIG(status));


int	exec(t_data *data)
{
	if (count_cmd(data->cmds) == 1)
	{
		dup_og(data);
		if (is_builtin(data->cmds))
		{
			if (handle_redirections(data->cmds, data))
				return (exit_status(1, 1), 1);
			exec_builtin(data->cmds, data);
		}
		else
		{
			exit_status(exec_single_cmd(data), 1);
			dup2_og(data);
		}
	}
	else
		exit_status(exec_multiple_cmd(data), 1);
	return (0);
}
