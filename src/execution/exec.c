/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:36:35 by anachat           #+#    #+#             */
/*   Updated: 2025/04/15 12:09:54 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function t_env * ---> char **

int	ft_dup2(int oldfd, int newfd)
{
	int	res;

	res = dup2(oldfd, newfd);
	close(oldfd);
	return (res);
}

int	exec_one_cmd(t_data *data)
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
		if (cmd->input_file)
		{
			int fd = open(cmd->input_file, O_RDONLY);
			if (fd < 0)
				return (perror("failed to open infile"), 1);
			ft_dup2(fd, 0);
		}
		if (cmd->output_file)
		{
			int fd = open(cmd->output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd < 0)
				return (perror("failed to open outfile"), 1);
			ft_dup2(fd, 1);
		}
		if (execve(cmd->path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve failed");
			exit(1);
		}
		return (0);
	}
	wait(NULL);
	return (0);
}


int	exec(t_data *data)
{
	// int	main(int ac, char **av, char **env)
	// pid_t	last_pid;
	// int		exit_st;
	// int		fd[7];
	// int		i;

	if (count_cmd(data->cmds) == 1)
	{
		if (is_builtin(data->cmds))
			return (exec_builtin(data->cmds, data), 0);
		return (exec_one_cmd(data));
	}
	

	
	// fd[3] = dup(0);
	// fd[4] = dup(1);
	// while (i < ac - 2)
	// 	parent1(fd, av, i++, env);
	// fd[6] = i;
	// exit_st = parent2(fd, av, env, &last_pid);
	// ft_dup2(fd[3], 0);
	// ft_dup2(fd[4], 1);
	// return (ft_wait(last_pid, exit_st));
	return (0);
}