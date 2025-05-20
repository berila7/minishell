/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/05/20 20:36:29 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_cmd	*parse_tokens(t_token *tokens, t_data *data)
// {
// 	t_token	*token;
// 	t_cmd	*current_cmd;
// 	t_cmd	*cmd_list;
// 	char	*expanded;

// 	// (void)expanded;
// 	if (!tokens)
// 		return (NULL);
// 	current_cmd = new_command(&data->gc);
// 	cmd_list = NULL;
// 	if (!current_cmd)
// 		return (NULL);
// 	token = tokens;
// 	while (token)
// 	{
// 		if (token->type == TOKEN_WORD)
// 		{
// 			process_token_word(&data->gc, token, current_cmd, data);
// 			token = token->next;
// 		}
// 		else if (token->type == TOKEN_PIPE)
// 		{
// 			add_command(&cmd_list, current_cmd, data);
// 			current_cmd = new_command(&data->gc);
// 			if (!current_cmd)
// 			{
// 				free_commands(&data->gc, cmd_list);
// 				return (NULL);
// 			}
// 			token = token->next;
// 		}
// 		else if (token->type == TOKEN_REDIR_IN)
// 		{
// 			token = token->next;
// 			if (token && token->type == TOKEN_WORD)
// 			{
// 				expanded = expand_variables(&data->gc, token->value, data);
// 				add_redirection(&data->gc, current_cmd, REDIR_IN, expanded);
// 				gc_free(&data->gc, expanded);
// 				token = token->next;
// 			}
// 			else
// 			{
// 				printf("minishell: syntax error near unexpected token 'newline'\n");
// 				exit_status(2, 1);
// 				free_commands(&data->gc, cmd_list);
// 				free_command(&data->gc, current_cmd);
// 				return (NULL);
// 			}
// 		}
// 		else if (token->type == TOKEN_REDIR_OUT)
// 		{
// 			token = token->next;
// 			if (token && token->type == TOKEN_WORD)
// 			{
// 				expanded = expand_variables(&data->gc, token->value, data);
// 				add_redirection(&data->gc, current_cmd, REDIR_OUT, expanded);
// 				gc_free(&data->gc, expanded);
// 				token = token->next;
// 			}
// 			else
// 			{
// 				printf("minishell: syntax error near unexpected token 'newline'\n");
// 				exit_status(2, 1);
// 				free_commands(&data->gc, cmd_list);
// 				free_command(&data->gc, current_cmd);
// 				return (NULL);
// 			}
// 		}
// 		else if (token->type == TOKEN_REDIR_APPEND)
// 		{
// 			token = token->next;
// 			if (token && token->type == TOKEN_WORD)
// 			{
// 				expanded = expand_variables(&data->gc, token->value, data);
// 				add_redirection(&data->gc, current_cmd, REDIR_APPEND, expanded);
// 				gc_free(&data->gc, expanded);
// 				token = token->next;
// 			}
// 			else
// 			{
// 				printf("minishell: syntax error near unexpected token 'newline'\n");
// 				exit_status(2, 1);
// 				free_commands(&data->gc, cmd_list);
// 				free_command(&data->gc, current_cmd);
// 				return (NULL);
// 			}
// 		}
// 		else if (token->type == TOKEN_HEREDOC)
// 		{
// 			token = token->next;
// 			if (token && token->type == TOKEN_WORD)
// 			{
// 				if (handle_herdoc(&data->gc, token->value, &current_cmd->hd_fd, data))
// 				{
// 					free_commands(&data->gc, cmd_list);
// 					free_command(&data->gc, current_cmd);
// 					return (NULL);
// 				}
// 				add_redirection(&data->gc, current_cmd, REDIR_HEREDOC, token->value);
// 				token = token->next;
// 			}
// 			else
// 			{
// 				printf("minishell: syntax error near unexpected token '<<'\n");
// 				exit_status(2, 1);
// 				free_commands(&data->gc, cmd_list);
// 				free_command(&data->gc, current_cmd);
// 				return (NULL);
// 			}
// 		}
// 	}
// 	add_command(&cmd_list, current_cmd, data);
// 	return (cmd_list);
// }


t_cmd *parse_tokens(t_token *tokens, t_data *data)
{
	t_token	*token;
	t_cmd	*current_cmd;
	t_cmd	*cmd_list;

	if (!tokens)
		return (NULL);
	current_cmd = new_command(&data->gc);
	cmd_list = NULL;
	if (!current_cmd)
		return (NULL);
	token = tokens;
	while (token)
	{
		if (!process_token(&token, &current_cmd, &cmd_list, data))
			return (NULL);
	}
	add_command(&cmd_list, current_cmd, data);
	return (cmd_list);
}

int process_token(t_token **token, t_cmd **current_cmd, t_cmd **cmd_list, t_data *data)
{
	if ((*token)->type == TOKEN_WORD)
		return (process_word_token(token, *current_cmd, data));
	else if ((*token)->type == TOKEN_PIPE)
		return (process_pipe_token(token, current_cmd, cmd_list, data));
	else if ((*token)->type == TOKEN_REDIR_IN)
		return (process_redir_token(token, *current_cmd, *cmd_list, REDIR_IN, data));
	else if ((*token)->type == TOKEN_REDIR_OUT)
		return (process_redir_token(token, *current_cmd, *cmd_list, REDIR_OUT, data));
	else if ((*token)->type == TOKEN_REDIR_APPEND)
		return (process_redir_token(token, *current_cmd, *cmd_list, REDIR_APPEND, data));
	else if ((*token)->type == TOKEN_HEREDOC)
		return (process_heredoc_token(token, *current_cmd, *cmd_list, data));
	return (1);
}

int process_word_token(t_token **token, t_cmd *current_cmd, t_data *data)
{
	process_token_word(&data->gc, *token, current_cmd, data);
	*token = (*token)->next;
	return (1);
}

int process_pipe_token(t_token **token, t_cmd **current_cmd, t_cmd **cmd_list, 
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

int process_redir_token(t_token **token, t_cmd *current_cmd, t_cmd *cmd_list,
                       int redir_type, t_data *data)
{
	char *expanded;

	*token = (*token)->next;
	if (*token && (*token)->type == TOKEN_WORD)
	{
		expanded = expand_variables(&data->gc, (*token)->value, data);
		add_redirection(&data->gc, current_cmd, redir_type, expanded);
		gc_free(&data->gc, expanded);
		*token = (*token)->next;
		return (1);
	}
	else
	{
		return (handle_redir_error(cmd_list, current_cmd, data));
	}
}

int handle_redir_error(t_cmd *cmd_list, t_cmd *current_cmd, t_data *data)
{
	printf("minishell: syntax error near unexpected token 'newline'\n");
	exit_status(2, 1);
	free_commands(&data->gc, cmd_list);
	free_command(&data->gc, current_cmd);
	return (0);
}

int process_heredoc_token(t_token **token, t_cmd *current_cmd, t_cmd *cmd_list,
                         t_data *data)
{
	*token = (*token)->next;
	if (*token && (*token)->type == TOKEN_WORD)
	{
		if (handle_herdoc(&data->gc, (*token)->value, &current_cmd->hd_fd, data))
		{
			free_commands(&data->gc, cmd_list);
			free_command(&data->gc, current_cmd);
			return (0);
		}
		add_redirection(&data->gc, current_cmd, REDIR_HEREDOC, (*token)->value);
		*token = (*token)->next;
		return (1);
	}
	else
	{
		printf("minishell: syntax error near unexpected token '<<'\n");
		exit_status(2, 1);
		free_commands(&data->gc, cmd_list);
		free_command(&data->gc, current_cmd);
		return (0);
	}
}