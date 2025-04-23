/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/04/23 13:42:05 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_single_cmd(t_data *data)
{
	pid_t	id;
	t_cmd	*cmd;

	cmd = data->cmds;
	if (!cmd->path)
		return (printf("%s: command not found\n", cmd->args[0]), 1);
	id = fork();
	if (id < 0)
		return (perror("fork failed"), 1);
	if (id == 0)
	{
		if (handle_redirections(data))
			return (1);
		if (execve(cmd->path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve failed");
			exit(1);
		}
		return (0);
	}
	return (ft_wait(id, 0));
}