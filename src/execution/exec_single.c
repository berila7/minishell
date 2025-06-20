/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/06/20 19:47:21 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execve(t_cmd *cmd, t_data *data)
{
	if (execve(cmd->path, cmd->args, env_to_array(&data->gc, data->env)) == -1)
	{
		if (errno == ENOEXEC)
			exit(0);
		if (errno == EACCES)
		{
			print_err("%s: Permission denied\n", cmd->path);
			exit(126);
		}
		if (errno == ENOENT)
		{
			print_err("%s: No such file or directory\n", cmd->args[0]);
			exit(127);
		}
		if (errno == ENOTDIR)
		{
			print_err("%s: Not a directory\n", cmd->args[0]);
			exit(126);
		}
		exit(1);
	}
	return (0);
}

static void	check_cmd(t_cmd *cmd, t_data *data, char *path_env)
{
	if (!ft_strchr(cmd->args[0], '/') || !cmd_exists(cmd->args[0])
		|| !is_exec(cmd->args[0]) || is_directory(cmd->args[0]))
	{
		if (!path_env || equal(path_env, ""))
			print_err("%s: No such file or directory\n", cmd->args[0]);
		else
			print_err("%s: command not found\n", cmd->args[0]);
		dup2_og(data);
		exit(127);
	}
}

int	handle_exec_errors(t_cmd *cmd, t_data *data)
{
	char	*path_env;

	if (count_args(cmd->args) == 0)
		return (dup2_og(data), exit(0), 0);
	path_env = get_env_path(data->env);
	if (equal(cmd->path, "..") && path_env && path_env[0] != '\0')
	{
		print_err("%s: command not found 1\n", cmd->args[0]);
		return (dup2_og(data), exit(127), 1);
	}
	else if (equal(cmd->args[0], "."))
	{
		if (path_env && !equal(path_env, ""))
			print_err("%s: command not found\n", cmd->args[0]);
		else
			print_err("%s: No such file or directory\n", cmd->args[0]);
		dup2_og(data);
		exit(127);
	}
	if (!cmd->path)
		check_cmd(cmd, data, path_env);
	else if (is_directory(cmd->args[0]))
		return (print_err("%s: Is a directory\n", cmd->args[0]),
			dup2_og(data), exit(126), 1);
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
		ft_execve(cmd, data);
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
