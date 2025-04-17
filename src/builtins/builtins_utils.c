/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:05 by anachat           #+#    #+#             */
/*   Updated: 2025/04/17 10:32:38 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	equal(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (0);
	if (ft_strcmp(s1, s2) == 0)
		return (1);
	return (0);
}

int	is_builtin(t_cmd *cmd)
{
	char	*name;

	name = cmd->args[0];
	if (equal(name, "echo") || equal(name, "cd") || equal(name, "pwd")
		|| equal(name, "export") || equal(name, "unset")
		|| equal(name, "env") || equal(name, "exit"))
		return (1);
	return (0);
}

int	open_outfile(char *file, int mode)
{
	int	fd;

	if (mode == 1)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	return (fd);
}

int handle_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd < 0)
			return (perror("failed to open infile"), 1);
		ft_dup2(fd, STDIN_FILENO);
	}
	if (cmd->output_file)
	{
		fd = open_outfile(cmd->output_file, cmd->append_mode);
		if (fd < 0)
			return (perror("failed to open outfile"), 1);
		ft_dup2(fd, STDOUT_FILENO);
	}
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_data *data)
{
	char	*name;
	int		fd[2];

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	if (handle_redirections(cmd))
		return (1);
	name = cmd->args[0];
	if (equal(name, "echo"))
		ft_echo(cmd->args);
	if (equal(name, "cd"))
		ft_cd(cmd->args, data);
	else if (equal(name, "pwd"))
		ft_pwd();
	else if (equal(name, "export"))
		ft_export(cmd->args, data);
	else if (equal(name, "unset"))
		ft_unset(cmd->args, data->env);
	else if (equal(name, "env"))
		ft_env(data->env);
	else if (equal(name, "exit"))
		ft_exit(cmd->args, data);
	ft_dup2(fd[0], STDIN_FILENO);
	ft_dup2(fd[1], STDOUT_FILENO);
	return (0);
}
