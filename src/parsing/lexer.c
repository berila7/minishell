/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:07:53 by mberila           #+#    #+#             */
/*   Updated: 2025/05/17 16:08:51 by berila           ###   ########.fr       */
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

void	extract_word(t_token **tokens, char *line, int *i, t_data *data)
{
	int		start;
	int		in_quote;
	char 	*word;
	// char	*expanded_word;
	char	*quote_state;
	start = *i;
	in_quote = 0;
	while (line[*i])
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
		if (!in_quote && (line[*i] == ' ' || line[*i] == '\t' || line[*i] == '|' || line[*i] == '<' || line[*i] == '>'))
			break ;
		(*i)++;
	}
	if (in_quote != 0)
	{
		if (in_quote == 1)
			quote_state = "single";
		else
			quote_state = "double";
		printf("minishell: syntax error: unclosed %s quote\n", 
				quote_state);
		free_tokens(*tokens);
		*tokens = NULL;
		return ;
	}
	if (*i > start)
	{
		word = ft_substr(line, start, *i - start);
		// expanded_word = expand_variables(word, data);
		// free(expanded_word);
		add_token(tokens, new_token(word, TOKEN_WORD));
		if (equal(word, "export"))
			data->is_export = 0;
		printf("export flag %d\n", data->is_export);
		printf("word %s\n", word);
		free(word);
	}
}

t_token	*tokenize(char *line, t_data *data)
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
			extract_word(&token, line, &i, data);
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
