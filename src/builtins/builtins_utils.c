/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:05 by anachat           #+#    #+#             */
/*   Updated: 2025/04/23 13:41:50 by mberila          ###   ########.fr       */
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

// In Parent:
// export with args
// cd
// exit
// unset

// In Child
// export no args

int handle_redirections(t_data *data)
{
	t_redir	*redir;
	int		hd_fd[2];
	int		fd;
	int		i;
	int flag = 0;

	i = 0;
	while (i < data->cmds->redir_count)
	{
		redir = &data->cmds->redirections[i];
		// type == IN
		if (redir->type == REDIR_IN)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd < 0)
				return (perror("failed to open infile"), 1);
			ft_dup2(fd, STDIN_FILENO);
		}
		// type == OUT
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (redir->type == REDIR_OUT)
				fd = open_outfile(redir->file, 0);
			else if (redir->type == REDIR_APPEND)
				fd = open_outfile(redir->file, 1);
			if (fd < 0)
				return (perror("failed to open infile"), 1);
			ft_dup2(fd, STDOUT_FILENO);
		}
		else if (redir->type == REDIR_HEREDOC)
		{
			handle_herdoc(redir->file, hd_fd, data);
			flag = 1;
		}
		i++;
	}
	if (flag)
		ft_dup2(hd_fd[0], STDIN_FILENO);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_data *data, int flag)
{
	char	*name;
	int		fd[2];

	if (flag)
	{
		fd[0] = dup(STDIN_FILENO);
		fd[1] = dup(STDOUT_FILENO);
		if (handle_redirections(data))
			return (1);
	}
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
	if (flag)
	{
		ft_dup2(fd[0], STDIN_FILENO);
		ft_dup2(fd[1], STDOUT_FILENO);
	}
	return (0);
}
