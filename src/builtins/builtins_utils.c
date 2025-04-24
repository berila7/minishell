/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:05 by anachat           #+#    #+#             */
/*   Updated: 2025/04/24 11:17:45 by anachat          ###   ########.fr       */
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



int	handle_heredocs(t_data *data, t_cmd *cmd, int *heredoc_fd)
{
	t_redir	*redir;
	int		i;

	i = 0;
	while (i < cmd->redir_count)
	{
		redir = &cmd->redirections[i];
		if (redir->type == REDIR_HEREDOC)
		{
			if (handle_herdoc(redir->file, heredoc_fd, data) != 0)
			{
				perror("heredoc failed");
				return (1);
			}
		}
		i++;
	}
	return (0);
}



int	handle_other_redirs(t_cmd *cmd, int *heredoc_fd)
{
	t_redir	*redir;
	int		i;
	int		fd;

	i = 0;
	while (i < cmd->redir_count)
	{
		redir = &cmd->redirections[i];
		if (redir->type == REDIR_IN)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd < 0)
				return (perror("failed to open infile"), 1);
			if (count_args(cmd->args) > 0)
				ft_dup2(fd, STDIN_FILENO);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			fd = open_outfile(redir->file,
				redir->type == REDIR_APPEND);
			if (fd < 0)
				return (perror("failed to open outfile"), 1);
			if (count_args(cmd->args) > 0)
				ft_dup2(fd, STDOUT_FILENO);
		}
		i++;
	}
	// Apply heredoc only if needed and command is not null
	if (heredoc_fd && count_args(cmd->args) > 0)
		ft_dup2(heredoc_fd[0], STDIN_FILENO);
	return (0);
}



int	handle_redirections(t_data *data, t_cmd *cmd)
{
	int	heredoc_fd[2];

	if (handle_heredocs(data, cmd, heredoc_fd) != 0)
		return (1);
	if (handle_other_redirs(cmd, heredoc_fd) != 0)
		return (1);
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
		if (handle_redirections(data, cmd))
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
