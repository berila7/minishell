/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multiple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/04/17 12:13:14 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int child1(t_cmd *cmd, int i, t_data *data, int *pid)
{
	pid_t id;

	(void)i;
	if (pipe(data->curr_pipe) < 0)
		return (perror("pipe failed"), 1);
	id = fork();
	if (id < 0)
		return (perror("fork failed"), 1);
	if (id == 0)
	{
		*pid = id;
		close(data->curr_pipe[0]); // in child we only use write end in parent we use read end;

		// pipe redirection:
		ft_dup2(data->curr_pipe[1], STDOUT_FILENO);

		if (handle_redirections(cmd))
			return (1);
		
		if (is_builtin(cmd))
		{
			exec_builtin(cmd, data, 0);
		}
		else if (execve(cmd->path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve failed");
			exit(1);
		}
		return (0);
	}
	else
	{
		close(data->curr_pipe[1]);
		ft_dup2(data->curr_pipe[0], STDIN_FILENO);
		return (0);
	}
}

int	exec_multiple_cmd(t_data *data)
{
	pid_t	last_pid;
	t_cmd	*cmd;
	int		i;

	last_pid = 0;
	cmd = data->cmds;
	if (!cmd->path)
		return (printf("%s: command not found\n", cmd->args[0]), 1);
	i = 0;
	while (cmd)
	{
		child1(cmd, i, data, &last_pid);
		cmd = cmd->next;
		i++;
	}
	while (wait(NULL) != -1)
		;
	return (0);
	// return (ft_wait(last_pid, 0));
}
