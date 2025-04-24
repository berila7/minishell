/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multiple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/04/24 10:55:52 by anachat          ###   ########.fr       */
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
		*pid = id;
		
		if (cmd->next)
		{
			close(data->pipe[0]);
			ft_dup2(data->pipe[1], STDOUT_FILENO);
		}

		if (handle_redirections(data, cmd))
			return (1);
			
		if (count_args(cmd->args) == 0)
		{
			if (cmd->redir_count == 0)
				return (printf("%s: command not found\n", cmd->args[0]), exit(1), 1);
			exit(0);
		}
		if (is_builtin(cmd))
			exec_builtin(cmd, data, 0);
		else if (execve(cmd->path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve failed");
			exit(1);
		}
	}
	else
	{
		close(data->pipe[1]);
		ft_dup2(data->pipe[0], STDIN_FILENO);
	}
	return (0);
}

int	exec_multiple_cmd(t_data *data)
{
	pid_t	last_pid;
	t_cmd	*cmd;

	last_pid = 0;
	cmd = data->cmds;

	int stdin_backup = dup(STDIN_FILENO);
	int stdout_backup = dup(STDOUT_FILENO);

	while (cmd)
	{
		child1(cmd, data, &last_pid);
		cmd = cmd->next;
	}
	
	while (wait(NULL) != -1)
		;

	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	return (0);
	// return (ft_wait(last_pid, 0));
}
