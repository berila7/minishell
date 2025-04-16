/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:43:36 by anachat           #+#    #+#             */
/*   Updated: 2025/04/16 13:42:41 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_child1(int *fd, char *path, char **cmd, char **env)
{
	pid_t	id;

	id = fork();
	if (id < 0)
		return (perror("fork failed"), ft_dup2(fd[0], 0), 1);
	if (id == 0)
	{
		ft_close2(fd[3], fd[4]);
		close(fd[0]);
		ft_dup2(fd[1], 1);
		if (execve(path, cmd, env) == -1)
		{
			perror("execve 1 failed");
			exit(1);
		}
		return (0);
	}
	else
		return (close(fd[1]), ft_dup2(fd[0], 0), 0);
}

static int	parent1(int *fd, char **av, int i, char **env)
{
	char	**cmd;
	char	*path;

	if (pipe(fd) < 0)
		return (perror("pipe failed"), 1);
	if (file_check(av, i, fd) != 0)
		return (close(fd[0]), close(fd[1]), 0);
	if (!*(av[i]))
		return (ft_perr("command not found"), close(fd[1]),
			close(fd[2]), ft_dup2(fd[0], 0), 1);
	cmd = ft_split(av[i], ' ');
	if (!cmd)
		return (ft_perr("allocation error"), close(fd[1]),
			close(fd[2]), ft_dup2(fd[0], 0), 1);
	path = get_path(cmd[0], env);
	if (!path)
	{
		perror("command not found");
		return (free_arr(cmd), close(fd[1]), close(fd[2]),
			ft_dup2(fd[0], 0), 1);
	}
	exec_child1(fd, path, cmd, env);
	return (free_arr(cmd), close(fd[1]), close(fd[0]), free(path), 0);
}

static pid_t	exec_child2(int *fd, char *path, char **cmd, char **env)
{
	pid_t	id;

	id = fork();
	if (id < 0)
		return (close(fd[5]), 1);
	if (id == 0)
	{
		ft_close2(fd[3], fd[4]);
		ft_dup2(fd[5], 1);
		if (execve(path, cmd, env) == -1)
			return (perror("execve 2 failed"), free_arr(cmd),
				free(path), exit(1), id);
	}
	return (id);
}

static int	parent2(int *fd, char **av, char **env, pid_t *pid)
{
	char	*path;
	char	**cmd;
	int		i;

	i = fd[6];
	if (is_heredoc(av[1]))
		fd[5] = open(av[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd[5] = open(av[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd[5] < 0)
		return (perror("error opening outfile"), 1);
	if (!*(av[i]))
		return (ft_perr("command not found"), close(fd[5]), 1);
	cmd = ft_split(av[i], ' ');
	if (!cmd)
		return (ft_perr("allocation error"), close(fd[5]), 1);
	path = get_path(cmd[0], env);
	if (!path)
	{
		perror("command not found");
		return (free_arr(cmd), close(fd[5]), 1);
	}
	*pid = exec_child2(fd, path, cmd, env);
	return (free_arr(cmd), free(path), close(fd[5]), 0);
}

