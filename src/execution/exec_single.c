/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/04/24 15:45:09 by anachat          ###   ########.fr       */
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
		if (handle_redirections(cmd))
			return (1);
		if (!cmd->path)
		{
			if (cmd->redir_count == 0)
				return (printf("%s: command not found\n", cmd->args[0]), exit(127), 1);
			exit(1);
		}
		if (!is_exec(cmd->path))
			return (printf("minishell: %s: Permission denied\n", cmd->path), exit(126), 1);
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
