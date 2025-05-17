/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:07:53 by mberila           #+#    #+#             */
/*   Updated: 2025/05/17 12:04:56 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_pipe(t_token **token, int *i)
{
	add_token(token, new_token("|", TOKEN_PIPE));
	(*i)++;
}

static void	process_input_redirection(t_token **token, char *line, int *i)
{
	if (line[*i + 1] == '<')
	{
		add_token(token, new_token("<<", TOKEN_HEREDOC));
		(*i) += 2;
	}
	else
	{
		add_token(token, new_token("<", TOKEN_REDIR_IN));
		(*i)++;
	}
}

static void	process_output_redirection(t_token **token, char *line, int *i)
{
	if (line[*i + 1] == '>')
	{
		add_token(token, new_token(">>", TOKEN_REDIR_APPEND));
		(*i) += 2;
	}
	else
	{
		add_token(token, new_token(">", TOKEN_REDIR_OUT));
		(*i)++;
	}
}

void	process_token_word(t_token *token, t_cmd *current_cmd, t_data *data)
{
	char	*expanded;
	int		is_first_arg;

	is_first_arg = (current_cmd->args[0] == NULL);
	expanded = expand_variables(token->value, data);
	
	// Handle quotation - Bash treats quoted commands differently
	if (has_quotes_in_token(token->value))
	{
		handle_quoted_word(expanded, current_cmd);
	}
	else
		handle_unquoted_word(expanded, current_cmd, is_first_arg ? token : NULL);
		
	free(expanded);
}

t_token	*tokenize(char *line)
{
	t_token	*token;
	int		i;

	token = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (line[i] == '|')
			process_pipe(&token, &i);
		else if (line[i] == '<')
			process_input_redirection(&token, line, &i);
		else if (line[i] == '>')
			process_output_redirection(&token, line, &i);
		else
			extract_word(&token, line, &i);
	}
	return (token);
}
