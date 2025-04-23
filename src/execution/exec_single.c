/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/04/23 17:29:13 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_single_cmd(t_data *data)
{
	pid_t	id;
	t_cmd	*cmd;

	cmd = data->cmds;
	// if (!cmd->path)
	// 	return (printf("%s: command not found\n", cmd->args[0]), 1);
	id = fork();
	if (id < 0)
		return (perror("fork failed"), 1);
	if (id == 0)
	{
		if (handle_redirections(data, cmd))
			return (1);

		if (!cmd->path && cmd->redirections[0].type != REDIR_HEREDOC)
			return (printf("%s: command not found\n", cmd->args[0]), 1);
		else if (!cmd->path)
			return (0);

		if (execve(cmd->path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve failed");
			exit(1);
		}
		return (0);
	}
	return (ft_wait(id, 0));
}