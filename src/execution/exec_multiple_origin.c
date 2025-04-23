/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multiple_origin.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/04/23 13:41:57 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int child1(t_cmd *cmd, int i, t_data *data, int *pid)
{
	pid_t id;

	if (cmd->next)
	{
		if (pipe(data->curr_pipe) < 0)
			return (perror("pipe failed"), 1);
	}
	id = fork();
	if (id < 0)
		return (perror("fork failed"), 1);
	if (id == 0)
	{
		*pid = id;

		if (i == 0) // FIRST cmd
		{
			ft_dup2(data->curr_pipe[1], STDOUT_FILENO);
			close(data->curr_pipe[0]); // close unused pipe
			close(data->curr_pipe[1]); // close unused pipe
			close(data->curr_pipe[0]); // close unused pipe
		}
		else if (!cmd->next) // LAST cmd
		{
				ft_dup2(data->prev_pipe[0], STDIN_FILENO);
				close(data->prev_pipe[1]); // close unused pipe
				close(data->prev_pipe[0]); // close unused pipe
				close(data->prev_pipe[1]); // close unused pipe
		}
		else // MIDDLE cmd
		{
			ft_dup2(data->prev_pipe[0], STDIN_FILENO);
			close(data->prev_pipe[1]); // close unused pipe
			ft_dup2(data->curr_pipe[1], STDOUT_FILENO);
			close(data->curr_pipe[0]); // close unused pipe
		}
		if (handle_redirections(data))
			return (1);
		// Close all pipe fds in child
		// if (i > 0) {
		// 	close(data->prev_pipe[0]);
		// 	close(data->prev_pipe[1]);
		// }
		// if (cmd->next) {
		// 	close(data->curr_pipe[0]);
		// 	close(data->curr_pipe[1]);
		// }
		
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
		if (i > 0)
		{
			close(data->prev_pipe[0]); // close unused pipe
			close(data->prev_pipe[1]); // close unused pipe
		}
		if (cmd->next)
		{
			data->prev_pipe[0] = data->curr_pipe[0]; // forward pipes
			data->prev_pipe[1] = data->curr_pipe[1]; // forward pipes
		}
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
