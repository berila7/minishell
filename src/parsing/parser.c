/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/05/25 16:48:39 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redir_error(t_gcnode **gc, t_cmd *current_cmd,
		t_cmd *cmd_list)
{
	printf("minishell: syntax error near unexpected token 'newline'\n");
	free_commands(gc, cmd_list);
	free_command(gc, current_cmd);
	return (0);
}

int	handle_redir(t_token **token, t_cmd *curr_cmd,
		t_cmd **cmd_list, t_data *data)
{
	t_token_type	type;
	char			*expanded;
	int				redir_type;

	type = (*token)->type;
	*token = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN_WORD)
		return (handle_redir_error(&data->gc, curr_cmd, *cmd_list));
	if (type == TOKEN_HEREDOC)
	{
		if (handle_herdoc(&data->gc, (*token)->value, &curr_cmd->hd_fd, data))
			return (free_commands(&data->gc, *cmd_list),
				free_command(&data->gc, curr_cmd), 0);
		redir_type = REDIR_HEREDOC;
		add_redirection(&data->gc, curr_cmd, redir_type, (*token)->value);
	}
	else
	{
		expanded = expand_variables(&data->gc, (*token)->value, data);
		redir_type = set_redir_type(type);
		add_redirection(&data->gc, curr_cmd, redir_type, expanded);
		gc_free(&data->gc, expanded);
	}
	*token = (*token)->next;
	return (1);
}

int	handle_pipe(t_token **token, t_cmd **current_cmd, t_cmd **cmd_list,
						t_data *data)
{
	add_command(cmd_list, *current_cmd, data);
	*current_cmd = new_command(&data->gc);
	if (!*current_cmd)
	{
		free_commands(&data->gc, *cmd_list);
		return (0);
	}
	*token = (*token)->next;
	return (1);
}

int	process_token(t_token **token, t_cmd **current_cmd,
		t_cmd **cmd_list, t_data *data)
{
	if ((*token)->type == TOKEN_WORD)
	{
		process_token_word(&data->gc, *token, *current_cmd, data);
		*token = (*token)->next;
	}
	else if ((*token)->type == TOKEN_PIPE)
	{
		if (!handle_pipe(token, current_cmd, cmd_list, data))
			return (0);
	}
	else if ((*token)->type == TOKEN_REDIR_IN
		|| (*token)->type == TOKEN_REDIR_OUT
		|| (*token)->type == TOKEN_REDIR_APPEND
		|| (*token)->type == TOKEN_HEREDOC)
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
	current_cmd = new_command(&data->gc);
	cmd_list = NULL;
	token = tokens;
	while (token)
	{
		if (!process_token(&token, &current_cmd, &cmd_list, data))
			return (NULL);
	}
	add_command(&cmd_list, current_cmd, data);
	return (cmd_list);
}
