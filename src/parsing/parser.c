/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 19:51:10 by berila           ###   ########.fr       */
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

static void	init_parser(t_cmd **current_cmd, t_cmd **cmd_list)
{
	*current_cmd = new_command();
	*cmd_list = NULL;
}

static int	handle_pipe(t_token **token, t_cmd **current_cmd, t_cmd **cmd_list)
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

static int	handle_redir_error(t_token_type type, t_cmd *current_cmd,
		t_cmd *cmd_list)
{
	if (type == TOKEN_REDIR_APPEND)
		printf("minishell: syntax error near unexpected token 'newline'\n");
	else if (type == TOKEN_HEREDOC)
		printf("minishell: syntax error near unexpected token '<<'\n");
	else
		printf("minishell: syntax error near unexpected token '\\n'\n");
	free_commands(cmd_list);
	free_command(current_cmd);
	return (0);
}

static int	set_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else
		return (REDIR_APPEND);
}

static int	handle_redir(t_token **token, t_cmd *current_cmd,
		t_cmd **cmd_list, t_data *data)
{
	t_token_type	type;
	char			*expanded;
	int				redir_type;

	type = (*token)->type;
	*token = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN_WORD)
		return (handle_redir_error(type, current_cmd, *cmd_list));
	if (type == TOKEN_HEREDOC)
	{
		if (handle_herdoc((*token)->value, &current_cmd->hd_fd, data))
		{
			return (free_commands(*cmd_list), free_command(current_cmd), 0);
		}
		redir_type = REDIR_HEREDOC;
	}
	else
	{
		expanded = expand_variables((*token)->value, data);
		redir_type = set_redir_type(type);
		add_redirection(current_cmd, redir_type, expanded);
		free(expanded);
	}
	*token = (*token)->next;
	return (1);
}

static int	process_token(t_token **token, t_cmd **current_cmd,
		t_cmd **cmd_list, t_data *data)
{
	if ((*token)->type == TOKEN_WORD)
	{
		process_token_word(*token, *current_cmd, data);
		*token = (*token)->next;
	}
	else if ((*token)->type == TOKEN_PIPE)
	{
		if (!handle_pipe(token, current_cmd, cmd_list))
			return (0);
	}
	else if ((*token)->type == TOKEN_REDIR_IN || (*token)->type == TOKEN_REDIR_OUT
		|| (*token)->type == TOKEN_REDIR_APPEND || (*token)->type == TOKEN_HEREDOC)
	{
		if (!handle_redir(token, *current_cmd, cmd_list, data))
			return (0);
	}
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens, t_data *data)
{
	t_token	*token;
	t_cmd	*current_cmd;
	t_cmd	*cmd_list;

	if (!tokens)
		return (NULL);
	init_parser(&current_cmd, &cmd_list);
	if (!current_cmd)
		return (NULL);
	token = tokens;
	while (token)
	{
		if (!process_token(&token, &current_cmd, &cmd_list, data))
			return (NULL);
	}
	add_command(&cmd_list, current_cmd);
	return (cmd_list);
}
