/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:05:03 by mberila           #+#    #+#             */
/*   Updated: 2025/06/11 15:22:22 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redir_token(t_token **token, t_cmd *current_cmd, t_cmd *cmd_list,
						t_data *data)
{
	char	*expanded;
	int		redir_type;

	redir_type = set_redir_type((*token)->type);
	*token = (*token)->next;
	if (*token && (*token)->type == TOKEN_WORD)
	{
		expanded = expand_variables(data, (*token)->value);
		add_redirection(&data->gc, current_cmd, redir_type,
			smart_quote_removal(data, expanded, *token));
		current_cmd->redirections[current_cmd->redir_count - 1].quoted = 0;
		if ((*token)->quote_type == 0 && ft_strchr((*token)->value, '$'))
			current_cmd->redirections[current_cmd->redir_count - 1].quoted = 1;
		gc_free(&data->gc, expanded);
		*token = (*token)->next;
		return (1);
	}
	else
	{
		if (current_cmd->hd_fd != -1)
			close(current_cmd->hd_fd);
		printf("minishell: syntax error near unexpected token 'newline'\n");
		free_commands(&data->gc, cmd_list);
		return (exit_status(2, 1), free_command(&data->gc, current_cmd), 0);
	}
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

int	process_token_word(t_token **token,
	t_cmd *current_cmd, t_data *data)
{
	char	*expanded;
	char	*unquoted;

	unquoted = (*token)->value;
	if (!data->is_export && has_mixed_format((*token)->value))
	{
		unquoted = process_mixed_quoted(data, (*token)->value);
		(*token)->quote_type = 0;
		data->expandable = 0;
	}
	expanded = expand_variables(data, unquoted);
	if (!expanded)
	{
		gc_free(&data->gc, expanded);
		return (0);
	}
	process_unquoted_token((*token), data, expanded, current_cmd);
	gc_free(&data->gc, expanded);
	*token = (*token)->next;
	return (1);
}

int	process_token(t_token **token, t_cmd **current_cmd,
	t_cmd **cmd_list, t_data *data)
{
	if ((*token)->type == TOKEN_WORD)
		return (process_token_word(token, *current_cmd, data));
	else if ((*token)->type == TOKEN_PIPE)
		return (handle_pipe(token, current_cmd, cmd_list, data));
	else if ((*token)->type == TOKEN_REDIR_IN)
		return (process_redir_token(token,
				*current_cmd, *cmd_list, data));
	else if ((*token)->type == TOKEN_REDIR_OUT)
		return (process_redir_token(token,
				*current_cmd, *cmd_list, data));
	else if ((*token)->type == TOKEN_REDIR_APPEND)
		return (process_redir_token(token,
				*current_cmd, *cmd_list, data));
	else if ((*token)->type == TOKEN_HEREDOC)
		return (process_heredoc_token(token,
				*current_cmd, *cmd_list, data));
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
	export_handler(&token, data);
	while (token)
	{
		if (!process_token(&token, &current_cmd, &cmd_list, data))
			return (NULL);
	}
	add_command(&cmd_list, current_cmd, data);
	return (cmd_list);
}
