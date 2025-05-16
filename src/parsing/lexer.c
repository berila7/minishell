/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:07:53 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 17:43:43 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

static void	handle_unclosed_quote(t_token **tokens, int in_quote)
{
	char	*quote_state;

	if (in_quote == 1)
		quote_state = "single";
	else
		quote_state = "double";
	printf("minishell: syntax error: unclosed %s quote\n", quote_state);
	free_tokens(*tokens);
	*tokens = NULL;
}

static int	process_quotes(char *line, int *i, int in_quote)
{
	if (line[*i] == '\'')
	{
		if (in_quote == 0)
			in_quote = 1;
		else if (in_quote == 1)
			in_quote = 0;
	}
	else if (line[*i] == '\"')
	{
		if (in_quote == 0)
			in_quote = 2;
		else if (in_quote == 2)
			in_quote = 0;
	}
	return (in_quote);
}

void	extract_word(t_token **tokens, char *line, int *i)
{
	int		start;
	int		in_quote;
	char	*word;

	start = *i;
	in_quote = 0;
	while (line[*i])
	{
		in_quote = process_quotes(line, i, in_quote);
		if (!in_quote && (line[*i] == ' ' || line[*i] == '\t'
				|| line[*i] == '|' || line[*i] == '<' || line[*i] == '>'))
			break ;
		(*i)++;
	}
	if (in_quote != 0)
	{
		handle_unclosed_quote(tokens, in_quote);
		return ;
	}
	if (*i > start)
	{
		word = ft_substr(line, start, *i - start);
		add_token(tokens, new_token(word, TOKEN_WORD));
		free(word);
	}
}

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
