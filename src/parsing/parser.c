/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/05/19 17:39:31 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_tokens(t_gcnode **gc, t_token *tokens, t_data *data)
{
	t_token	*token;
	t_cmd	*current_cmd;
	t_cmd	*cmd_list;
	char	*expanded;

	// (void)expanded;
	if (!tokens)
		return (NULL);
	current_cmd = new_command(gc);
	cmd_list = NULL;
	if (!current_cmd)
		return (NULL);
	token = tokens;
	while (token)
	{
		if (token->type == TOKEN_WORD)
		{
			process_token_word(gc, token, current_cmd, data);
			token = token->next;
		}
		else if (token->type == TOKEN_PIPE)
		{
			add_command(&cmd_list, current_cmd, data);
			current_cmd = new_command(gc);
			if (!current_cmd)
			{
				free_commands(gc, cmd_list);
				return (NULL);
			}
			token = token->next;
		}
		else if (token->type == TOKEN_REDIR_IN)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				expanded = expand_variables(gc, token->value, data);
				add_redirection(gc, current_cmd, REDIR_IN, expanded);
				gc_free(gc, expanded);
				token = token->next;
			}
			else
			{
				printf("minishell: syntax error near unexpected token 'newline'\n");
				exit_status(2, 1);
				free_commands(gc, cmd_list);
				free_command(gc, current_cmd);
				return (NULL);
			}
		}
		else if (token->type == TOKEN_REDIR_OUT)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				expanded = expand_variables(gc, token->value, data);
				add_redirection(gc, current_cmd, REDIR_OUT, expanded);
				gc_free(gc, expanded);
				token = token->next;
			}
			else
			{
				printf("minishell: syntax error near unexpected token 'newline'\n");
				exit_status(2, 1);
				free_commands(gc, cmd_list);
				free_command(gc, current_cmd);
				return (NULL);
			}
		}
		else if (token->type == TOKEN_REDIR_APPEND)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				expanded = expand_variables(gc, token->value, data);
				add_redirection(gc, current_cmd, REDIR_APPEND, expanded);
				gc_free(gc, expanded);
				token = token->next;
			}
			else
			{
				printf("minishell: syntax error near unexpected token 'newline'\n");
				exit_status(2, 1);
				free_commands(gc, cmd_list);
				free_command(gc, current_cmd);
				return (NULL);
			}
		}
		else if (token->type == TOKEN_HEREDOC)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				if (handle_herdoc(gc, token->value, &current_cmd->hd_fd, data))
				{
					free_commands(gc, cmd_list);
					free_command(gc, current_cmd);
					return (NULL);
				}
				add_redirection(gc, current_cmd, REDIR_HEREDOC, token->value);
				token = token->next;
			}
			else
			{
				printf("minishell: syntax error near unexpected token '<<'\n");
				exit_status(2, 1);
				free_commands(gc, cmd_list);
				free_command(gc, current_cmd);
				return (NULL);
			}
		}
	}
	add_command(&cmd_list, current_cmd, data);
	return (cmd_list);
}
