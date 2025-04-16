/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:07:53 by mberila           #+#    #+#             */
/*   Updated: 2025/04/16 10:24:42 by mberila          ###   ########.fr       */
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

void	extract_word(t_token **tokens, char *line, int *i, t_env *env, int exit_status)
{
	int		start;
	int		in_quote;
	char 	*word;
	int		escaped;
	char	*expanded_word;
	char	*quote_state;

	escaped = 0;
	start = *i;
	in_quote = 0;
	while (line[*i])
	{
		if (line[*i] == '\\' && !escaped && !(in_quote == 1))
		{
			escaped = 1;
			(*i)++;
			continue ;
		}
		if (line[*i] == '\'' && !escaped && !in_quote)
			in_quote = 1;
		if (line[*i] == '\'' && !escaped && in_quote == 1)
			in_quote = 0;
		else if (line[*i] == '\"' && !escaped && !in_quote)
			in_quote = 2;
		else if (line[*i] == '\"' && !escaped && in_quote == 2)
			in_quote = 0;
		if (!escaped && !in_quote && (line[*i] == ' ' || line[*i] == '\t' || line[*i] == '|' || line[*i] == '<' || line[*i] == '>'))
			break ;
		escaped = 0;
		(*i)++;
	}
	if (in_quote != 0)
	{
		if (in_quote == 1)
			quote_state = "single";
		else
			quote_state = "double";
		printf(RED"minishell: syntax error: unclosed %s quote\n"RESET, 
				quote_state);
		free_tokens(*tokens);
		*tokens = NULL;
		return ;
	}
	if (*i > start)
	{
		word = ft_substr(line, start, *i - start);
		expanded_word = expand_variables(word, env, exit_status);
		free(word);
		add_token(tokens, new_token(expanded_word, TOKEN_WORD));
		free(expanded_word);
	}
}

t_token	*tokenize(char *line, t_env *env, int exit_status)
{
	t_token	*token = NULL;
	int		i;

	i = 0;
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (line[i] == '|')
		{
			add_token(&token, new_token("|", TOKEN_PIPE));
			i++;
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				add_token(&token, new_token("<<", TOKEN_HEREDOC));
				i += 2;
			}
			else
			{
				add_token(&token, new_token("<", TOKEN_REDIR_IN));
				i++;
			}				
		}
		else if (line[i] == '>')
		{
			if (line[i + 1] == '>')
			{
				add_token(&token, new_token(">>", TOKEN_REDIR_APPEND));
				i += 2;
			}
			else
			{
				add_token(&token, new_token(">", TOKEN_REDIR_OUT));
				i++;
			}
		}
		else
			extract_word(&token, line, &i, env, exit_status);
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
