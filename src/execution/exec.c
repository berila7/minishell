/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:36:35 by anachat           #+#    #+#             */
/*   Updated: 2025/06/26 15:09:26 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_hds(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmds;
	while (cmd)
	{
		if (cmd->hd_fd != -1)
			close(cmd->hd_fd);
		cmd = cmd->next;
	}
}

int	is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
		return (S_ISDIR(st.st_mode));
	return (0);
}

static void	print_sig_msg(int status, int *sig_printed)
{
	if (WTERMSIG(status) == SIGINT && !*sig_printed)
	{
		write(STDOUT_FILENO, "\n", 1);
		*sig_printed = 1;
	}
	else if (WTERMSIG(status) == SIGQUIT && !*sig_printed)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		*sig_printed = 1;
	}
}

int	ft_wait(pid_t last_pid, int default_st)
{
	pid_t	pid;
	int		status;
	int		exit_st;
	int		sig_printed;

	sig_printed = 0;
	exit_st = default_st;
	pid = wait(&status);
	while (pid != -1)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_st = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				exit_st = 128 + WTERMSIG(status);
				print_sig_msg(status, &sig_printed);
			}
		}
		pid = wait(&status);
	}
	return (exit_st);
}

int	exec(t_data *data)
{
	setup_parent_waiting_signals();
	if (count_cmd(data->cmds) == 1)
	{
		dup_og(data);
		if (is_builtin(data->cmds))
		{
			if (handle_redirections(data->cmds, data))
				return (exit_status(1, 1), 1);
			exec_builtin(data->cmds, data, 1);
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
