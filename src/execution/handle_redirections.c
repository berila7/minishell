/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 18:36:21 by anachat           #+#    #+#             */
/*   Updated: 2025/06/25 16:56:55 by anachat          ###   ########.fr       */
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
		if (open_infile(redir->file, cmd))
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

int	handle_other_redirs(t_cmd *cmd, int *is_hd_last)
{
	int	last_input_type;
	int	i;

	i = 0;
	*is_hd_last = 0;
	last_input_type = -1;
	while (i < cmd->redir_count)
	{
		if (cmd->redirections[i].type == REDIR_IN
			|| cmd->redirections[i].type == REDIR_HEREDOC)
			last_input_type = cmd->redirections[i].type;
		if (process_redir(&cmd->redirections[i], cmd))
			return (1);
		i++;
	}
	if (last_input_type == REDIR_HEREDOC)
		*is_hd_last = 1;
	return (0);
}

int	handle_redirections(t_cmd *cmd, t_data *data)
{
	int	is_hd_last;

	if (handle_other_redirs(cmd, &is_hd_last))
	{
		close2(data->pipe);
		dup2_og(data);
		close_hds(data);
		return (1);
	}
	close_other_hds(data->cmds, cmd);
	if (cmd->hd_fd != -1 && is_hd_last && count_args(cmd->args) > 0)
		ft_dup2(cmd->hd_fd, STDIN_FILENO);
	return (0);
}
