/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multiple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/05/04 16:05:51 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int child1(t_cmd *cmd, t_data *data, int *pid)
{
	pid_t id;

	if (pipe(data->pipe) < 0)
		return (perror("pipe failed"), 1);
	id = fork();
	if (id < 0)
		return (perror("fork failed"), 1);
	if (id == 0)
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
			exec_builtin(cmd, data, 0);
			// close(data->pipe[0]);
			// close(data->pipe[1]);
			// dup2_og(data);
			check_fds_in_child(cmd->args[0]);
			exit(data->exit_status);
		}
			
		if (!cmd->path)
		{
			dup2_og(data);
			if (count_args(cmd->args) > 0)
				return (print_err(": command not found\n", cmd->args[0]), exit(127), 1);
			exit(1);
		}
		if (!is_exec(cmd->path))
		{
			dup2_og(data);
			if (cmd->next)
				check_fds_in_child(cmd->args[0]);
			return (print_err(": Permission denied\n", cmd->path), exit(126), 1);
		}
		close(data->og_fd[0]);
		close(data->og_fd[1]);
		if (execve(cmd->path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve failed");
			exit(1);
		}
	}
	else
	{
		if (!cmd->next)
			*pid = id;
		close(data->pipe[1]);
		ft_dup2(data->pipe[0], STDIN_FILENO);
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
		cmd = cmd->next;
	}

	exit_status = ft_wait(last_pid, 0);
	return (exit_status);
}
