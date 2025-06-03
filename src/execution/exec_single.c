/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/05/30 17:33:02 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_execve(t_cmd *cmd, t_data *data)
{
	if (execve(cmd->path, cmd->args, env_to_array(&data->gc, data->env)) == -1)
		return (perror("execve failed"), exit(1), 1);
	return (0);
}

int handle_exec_errors(t_cmd *cmd, t_data *data)
{
	DIR		*dir;
	char	*is_path;

	is_path = ft_strchr(cmd->args[0], '/');
	dir = opendir(cmd->args[0]);
	if (is_path && dir)
		return (dup2_og(data), closedir(dir),
			print_err("%s: Is a directory\n", cmd->args[0]), exit(126), 1);
	if (is_path && access(cmd->args[0], F_OK) == -1)
		return (perror(cmd->args[0]), exit(127), 1);
	if (!cmd->path)
	{
		dup2_og(data);
		if (count_args(cmd->args) > 0)
			return (print_err("%s: command not found\n", cmd->args[0]),
				exit(127), 1);
		exit(1);
	}
	if (!is_exec(cmd->path))
		return (dup2_og(data),
			print_err("%s: Permission denied\n", cmd->path), exit(126), 1);
	return (0);
}

static pid_t	exec_cmd(t_cmd *cmd, t_data *data)
{
	pid_t	id;

	id = fork();
	if (id < 0)
		return (perror("fork failed"), exit_status(1, 1), dup2_og(data), 1);
	if (id == 0)
	{
		setup_child_default_signals();
		if (handle_redirections(cmd, data))
			exit(1);
		handle_exec_errors(cmd, data);
		close2(data->og_fd);
		return (ft_execve(cmd, data));
	}
	return (id);
}

int	exec_single_cmd(t_data *data)
{
	pid_t	id;
	t_cmd	*cmd;

	cmd = data->cmds;
	id = exec_cmd(cmd, data);
	if (cmd->hd_fd != -1)
		close(cmd->hd_fd);
	return (ft_wait(id, 0));
}
