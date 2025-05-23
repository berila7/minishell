/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:05 by anachat           #+#    #+#             */
/*   Updated: 2025/05/23 14:59:54 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_other_redirs(t_cmd *cmd)
{
	t_redir	*redir;
	int		fd;
	int		i;

	i = 0;
	while (i < cmd->redir_count)
	{
		redir = &cmd->redirections[i];
		if (redir->type == REDIR_IN)
		{
			if (open_infile(redir->file, cmd))
				return (1);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			fd = open_outfile(redir->file,
					redir->type == REDIR_APPEND);
			if (fd < 0)
				return (perror(redir->file), 1);
			if (count_args(cmd->args) > 0)
				ft_dup2(fd, STDOUT_FILENO);
		}
		i++;
	}
	return (0);
}

void	close_hds(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmds;
	while (cmd)
	{
		if (cmd->hd_fd != -1)
			close(cmd->hd_fd);
		cmd = cmd->next;
	}
}

void	close_other_hds(t_cmd *cmds, t_cmd *current_cmd)
{
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (cmd != current_cmd && cmd->hd_fd >= 0)
		{
			close(cmd->hd_fd);
			cmd->hd_fd = -1;
		}
		cmd = cmd->next;
	}
}

int	handle_redirections(t_cmd *cmd, t_data *data)
{
	if (handle_other_redirs(cmd))
	{
		close2(data->pipe);
		dup2_og(data);
		close_hds(data);
		return (1);
	}
	close_other_hds(data->cmds, cmd);
	if (cmd->hd_fd != -1 && count_args(cmd->args) > 0)
		ft_dup2(cmd->hd_fd, STDIN_FILENO);
	return (0);
}

void	exec_builtin(t_cmd *cmd, t_data *data)
{
	char	*name;
	int		ext_status;

	name = cmd->args[0];
	if (equal(name, "echo"))
		ft_echo(cmd->args);
	else if (equal(name, "cd"))
		ft_cd(cmd->args, data);
	else if (equal(name, "pwd"))
		ft_pwd(data);
	else if (equal(name, "export"))
		ft_export(cmd->args, data);
	else if (equal(name, "unset"))
		ft_unset(cmd->args, data);
	else if (equal(name, "env"))
		ft_env(data->env);
	else if (equal(name, "exit"))
		ft_exit(cmd->args);
	dup2_og(data);
	if (equal(name, "exit"))
	{
		gc_free_all(&data->gc);
		ext_status = exit_status(0, 0);
		exit(ext_status);
	}
}
