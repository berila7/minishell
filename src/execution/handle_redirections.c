/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 18:36:21 by anachat           #+#    #+#             */
/*   Updated: 2025/06/26 15:16:00 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_redir(t_redir *redir, t_cmd *cmd)
{
	int	fd;

	if (redir->quoted)
		return (print_err("%s: ambiguous redirect\n", "minishell"), 1);
	if (redir->type == REDIR_IN)
	{
		if (open_infile(redir->file))
			return (1);
	}
	else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
	{
		fd = open_outfile(redir->file, redir->type == REDIR_APPEND);
		if (fd < 0)
			return (perror(redir->file), 1);
		if (count_args(cmd->args) > 0)
			ft_dup2(fd, STDOUT_FILENO);
	}
	return (0);
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

static int	is_last_heredoc(t_cmd *cmd, int current_index)
{
	int	j;

	j = current_index + 1;
	while (j < cmd->redir_count)
	{
		if (cmd->redirections[j].type == REDIR_HEREDOC)
			return (0);
		j++;
	}
	return (1);
}

int	handle_other_redirs(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirections[i].type == REDIR_HEREDOC)
		{
			if (is_last_heredoc(cmd, i) && cmd->hd_fd >= 0)
				ft_dup2(cmd->hd_fd, STDIN_FILENO);
		}
		else if (process_redir(&cmd->redirections[i], cmd))
			return (1);
		i++;
	}
	return (0);
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
	return (0);
}
