/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:37:53 by berila            #+#    #+#             */
/*   Updated: 2025/05/25 16:48:50 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_command(t_gcnode **gc)
{
	t_cmd	*cmd;

	cmd = gc_malloc(gc, sizeof(t_cmd));
	cmd->args = gc_malloc(gc, sizeof(char *) * 2);
	cmd->hd_fd = -1;
	if (!cmd->args)
	{
		gc_free(gc, cmd);
		return (NULL);
	}
	cmd->args[0] = NULL;
	cmd->redirections = NULL;
	cmd->redir_count = 0;
	return (cmd);
}

void	add_argument(t_gcnode **gc, t_cmd *cmd, char *arg)
{
	char	**new_args;
	int		i;
	int		size;

	i = 0;
	size = 0;
	while (cmd->args[i])
	{
		i++;
		size++;
	}
	new_args = gc_malloc(gc, sizeof(char *) * (size + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = gc_strdup(gc, remove_quotes(gc, arg));
	new_args[i + 1] = NULL;
	gc_free(gc, cmd->args);
	cmd->args = new_args;
}

void	add_redirection(t_gcnode **gc, t_cmd *cmd, int type, char *file)
{
	t_redir	*new_redirs;
	int		i;

	new_redirs = gc_malloc(gc, sizeof(t_redir) * (cmd->redir_count + 1));
	if (!new_redirs)
		return ;
	i = 0;
	while (i < cmd->redir_count)
	{
		new_redirs[i].type = cmd->redirections[i].type;
		new_redirs[i].file = cmd->redirections[i].file;
		i++;
	}
	new_redirs[cmd->redir_count].type = type;
	new_redirs[cmd->redir_count].file = gc_strdup(gc, file);
	gc_free(gc, cmd->redirections);
	cmd->redirections = new_redirs;
	cmd->redir_count++;
}

void	add_command(t_cmd **cmds, t_cmd *new_cmd, t_data *data)
{
	t_cmd	*current;

	data->is_export = 1;
	if (!*cmds)
	{
		*cmds = new_cmd;
		return ;
	}
	current = *cmds;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

int	set_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else
		return (REDIR_APPEND);
}
