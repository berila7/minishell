/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/05/01 18:13:12 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	exec_cmd(t_cmd *cmd, t_data *data)
{
	pid_t	id;

	id = fork();
	if (id < 0)
		return (perror("fork failed"), data->exit_status = 1, 1);
	if (id == 0)
	{
		if (handle_redirections(cmd, data))
			return (1);
		if (!cmd->path)
		{
			dup2_og(data);
			if (count_args(cmd->args) > 0)
				return (print_err(": command not found\n", cmd->args[0]), exit(127), 1);
			exit(0);
		}
		if (!is_exec(cmd->path))
		{	
			dup2_og(data);
			return (print_err(": Permission denied\n", cmd->path), exit(126), 1);
		}
		check_fds_in_child(cmd->args[0]);
		if (execve(cmd->path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve failed");
			exit(1);
		}
	}
	return (id);
}

int	exec_single_cmd(t_data *data)
{
	pid_t	id;
	t_cmd	*cmd;

	cmd = data->cmds;
	id = exec_cmd(cmd, data);
	return (ft_wait(id, 0));
}
