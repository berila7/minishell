/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:07:53 by mberila           #+#    #+#             */
/*   Updated: 2025/05/31 19:07:48 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	extract_word(t_token **tokens, char *line, int *i, t_data *data)
{
	int		start;
	int		in_quote;
	char	*raw_word_from_substr;
	char    *normalized_segment_value; // Using a more descriptive name
	t_token *the_new_token;

	start = *i;
	// toggel_quote identifies the boundaries of the raw word segment, respecting outer quotes
	in_quote = toggel_quote(line, i); 
	if (handle_quote_error(tokens, data, in_quote)) // Handles unclosed outer quotes
		return ;

	if (*i > start) // A segment was found
	{
		// Extract the raw segment as identified by toggel_quote
		raw_word_from_substr = gc_substr(&data->gc, line, start, *i - start);

		// Normalize this raw segment (e.g., "x"hello''world -> xhelloworld)
		normalized_segment_value = normalize_word_segment(&data->gc, raw_word_from_substr);
		
		// Free the raw segment, as we now have the normalized version
		gc_free(&data->gc, raw_word_from_substr);

		// Create a token with the fully normalized string value.
		// new_token will gc_strdup normalized_segment_value.
		the_new_token = new_token(&data->gc, normalized_segment_value, TOKEN_WORD);
		
		// Free normalized_segment_value as new_token has its own copy.
		gc_free(&data->gc, normalized_segment_value);

		if (the_new_token) {
			add_token(tokens, the_new_token);
			// export_handler is called with the head of the token list, its behavior
			// with respect to the newly added token depends on its internal logic.
			export_handler(tokens, data); 
		}
	}
}

int	handle_redout_tokens(t_data *data, t_token **token, char *line, int *i)
{
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			add_token(token, new_token(&data->gc, ">>", TOKEN_REDIR_APPEND));
			(*i) += 2;
		}
		else
		{
			add_token(token, new_token(&data->gc, ">", TOKEN_REDIR_OUT));
			(*i)++;
		}
		return (1);
	}
	return (0);
}

int	handle_redin_tokens(t_data *data, t_token **token, char *line, int *i)
{
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			add_token(token, new_token(&data->gc, "<<", TOKEN_HEREDOC));
			data->hered_count++;
			if (data->hered_count > 16)
			{
				print_err("minishell: maximum\
					here-document count exceeded\n", NULL);
				gc_free_all(&data->gc);
				exit(2);
			}
			(*i) += 2;
		}
		else
		{
			add_token(token, new_token(&data->gc, "<", TOKEN_REDIR_IN));
			(*i)++;
		}
		return (1);
	}
	return (0);
}

int	handle_tokens(t_data *data, t_token **token, char *line, int *i)
{
	if (line[*i] == '|')
	{
		add_token(token, new_token(&data->gc, "|", TOKEN_PIPE));
		(*i)++;
		return (1);
	}
	else if (!handle_redin_tokens(data, token, line, i)
		&& !handle_redout_tokens(data, token, line, i))
		return (0);
	return (1);
}

t_token	*tokenize(char *line, t_data *data)
{
	t_token	*token;
	int		i;

	i = 0;
	token = NULL;
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (!handle_tokens(data, &token, line, &i))
			extract_word(&token, line, &i, data);
	}
	return (token);
}
