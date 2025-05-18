/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multiple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/05/18 17:54:58 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_cmd(t_cmd *cmd, t_data *data)
{
	if (!cmd->path)
	{
		dup2_og(data);
		if (count_args(cmd->args) > 0)
			return (print_err("%s: command not found\n", cmd->args[0]),
				exit(127), 1);
		exit(1);
	}
	if (!is_exec(cmd->path))
	{
		dup2_og(data);
		return (print_err("%s: Permission denied\n", cmd->path),
			exit(126), 1);
	}
	close2(data->og_fd);
	if (execve(cmd->path, cmd->args, env_to_array(&data->gc, data->env)) == -1)
		return (perror("execve failed"), exit(1), 1);
	return (0);
}

void	exec_child(t_cmd *cmd, t_data *data)
{
	if (cmd->next)
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
		exec_cmd(cmd, data);
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
	int		exit_status;
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
	exit_status = ft_wait(last_pid, 0);
	dup2_og(data);
	return (exit_status);
}
