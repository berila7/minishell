/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multiple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/06/03 17:50:38 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child(t_cmd *cmd, t_data *data)
{
	setup_child_default_signals();
	if (cmd && cmd->next)
	{
		close(data->pipe[0]);
		ft_dup2(data->pipe[1], STDOUT_FILENO);
	}
	if (handle_redirections(cmd, data))
		exit(1);
	if (is_builtin(cmd))
	{
		exec_builtin(cmd, data);
		exit(exit_status(0, 0));
	}
	else
	{
		handle_exec_errors(cmd, data);
		close2(data->og_fd);
		ft_execve(cmd, data);
	}
}

int	child1(t_cmd *cmd, t_data *data, int *pid)
{
	pid_t	id;

	if (cmd->next && pipe(data->pipe) < 0)
		return (perror("pipe failed"), 1);
	id = fork();
	if (id < 0)
		return (perror("fork failed"), 1);
	if (id == 0)
		exec_child(cmd, data);
	else
	{
		if (!cmd->next)
			*pid = id;
		return (close(data->pipe[1]), ft_dup2(data->pipe[0], STDIN_FILENO), 0);
	}
	return (0);
}

int	exec_multiple_cmd(t_data *data)
{
	int		exit_st;
	pid_t	last_pid;
	t_cmd	*cmd;

	last_pid = 0;
	cmd = data->cmds;
	data->og_fd[0] = dup(STDIN_FILENO);
	data->og_fd[1] = dup(STDOUT_FILENO);
	while (cmd)
	{
		child1(cmd, data, &last_pid);
		if (cmd->hd_fd != -1)
			close(cmd->hd_fd);
		cmd = cmd->next;
	}
	close_hds(data);
	dup2_og(data);
	exit_st = ft_wait(last_pid, 0);
	exit_status(1, exit_st);
	return (exit_st);
}
