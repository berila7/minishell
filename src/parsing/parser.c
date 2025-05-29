/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/05/29 17:04:45 by mberila          ###   ########.fr       */
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
		expanded = expand_variables(&data->gc, (*token)->value, data);
		add_redirection(&data->gc, current_cmd, redir_type, expanded);
		if ((*token)->quote_type > 0)
			current_cmd->redirections->quoted = 1;
		gc_free(&data->gc, expanded);
		*token = (*token)->next;
		return (1);
	}
	else
	{
		printf("minishell: syntax error near unexpected token 'newline'\n");
		exit_status(2, 1);
		free_commands(&data->gc, cmd_list);
		free_command(&data->gc, current_cmd);
		return (0);
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

void	process_quoted_token(t_token *token, t_gcnode **gc, char *expanded,
	t_cmd *current_cmd)
{
	// char	*final_str;

	// final_str = remove_quotes(gc, expanded);
	add_argument(token, gc, current_cmd, expanded);
	// free(final_str);
}

void	process_token_word(t_gcnode **gc, t_token *token,
	t_cmd *current_cmd, t_data *data)
{
	char	*expanded;

	expanded = expand_variables(gc, token->value, data);
	if (!expanded || (ft_strlen(expanded) == 0))
	{
		gc_free(gc, expanded);
		return ;
	}
	if (!token->splited || !data->is_export)
		process_quoted_token(token, gc, expanded, current_cmd);
	else
		process_unquoted_token(token, gc, expanded, current_cmd);
	gc_free(gc, expanded);
}

int	process_word_token(t_token **token, t_cmd *current_cmd, t_data *data)
{
	process_token_word(&data->gc, *token, current_cmd, data);
	*token = (*token)->next;
	return (1);
}

int	process_token(t_token **token, t_cmd **current_cmd,
	t_cmd **cmd_list, t_data *data)
{
	if ((*token)->type == TOKEN_WORD)
		return (process_word_token(token, *current_cmd, data));
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
	while (token)
	{
		if (!process_token(&token, &current_cmd, &cmd_list, data))
			return (NULL);
	}
	add_command(&cmd_list, current_cmd, data);
	return (cmd_list);
}
