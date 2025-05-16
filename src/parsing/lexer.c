/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:07:53 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 18:41:39 by mberila          ###   ########.fr       */
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

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}
