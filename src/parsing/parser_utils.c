/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:46:58 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 20:17:14 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_command(t_cmd **cmds, t_cmd *new_cmd)
{
	t_cmd	*current;

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

int	handle_pipe(t_token **token, t_cmd **current_cmd, t_cmd **cmd_list)
{
	add_command(cmd_list, *current_cmd);
	*current_cmd = new_command();
	if (!*current_cmd)
	{
		free_commands(*cmd_list);
		return (0);
	}
	*token = (*token)->next;
	return (1);
}

t_cmd	*new_command(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->args = malloc(sizeof(char *) * 2);
	cmd->hd_fd = -1;
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args[0] = NULL;
	cmd->redirections = NULL;
	cmd->redir_count = 0;
	return (cmd);
}

void	add_argument(t_cmd *cmd, char *arg)
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
	new_args = malloc(sizeof(char *) * (size + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

void	add_redirection(t_cmd *cmd, int type, char *file)
{
	t_redir	*new_redirs;
	int		i;

	new_redirs = malloc(sizeof(t_redir) * (cmd->redir_count + 1));
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
	new_redirs[cmd->redir_count].file = ft_strdup(file);
	free(cmd->redirections);
	cmd->redirections = new_redirs;
	cmd->redir_count++;
}
