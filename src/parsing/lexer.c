/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:07:53 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 20:11:06 by mberila          ###   ########.fr       */
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

	expanded = expand_variables(token->value, data);
	if (has_quotes_in_token(token->value))
		handle_quoted_word(expanded, current_cmd);
	else
		handle_unquoted_word(expanded, current_cmd);
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
