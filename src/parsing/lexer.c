/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:07:53 by mberila           #+#    #+#             */
/*   Updated: 2025/06/02 11:19:34 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	extract_word(t_token **tokens, char *line, int *i, t_data *data)
{
	int		start_index;
	int		end_index; // To store the position *i after toggel_quote
	int		quote_status_at_end;
	char	*raw_segment_from_substr;
	char	*final_value_for_new_token;
	t_token *the_new_token;

	start_index = *i;
	// toggel_quote advances *i to the delimiter or EOL past the word segment
	quote_status_at_end = toggel_quote(line, i); 
	if (handle_quote_error(tokens, data, quote_status_at_end))
		return ;

	end_index = *i; // Current *i is the end of the segment + 1
	
	if (end_index > start_index) // A segment was indeed found
	{
		raw_segment_from_substr = gc_substr(&data->gc, line, start_index, end_index - start_index);

		if (is_simple_non_empty_quoted_string(raw_segment_from_substr)) {
			// If it's like "hello world" or 'foo' or even "" or '',
			// the token value should be the raw segment itself (with its quotes).
			final_value_for_new_token = gc_strdup(&data->gc, raw_segment_from_substr);
		} else {
			// For complex concatenations ("x"hello), internal empty quotes (he""llo),
			// or plain unquoted words (foobar), process aggressively.
			final_value_for_new_token = process_complex_segment(data, raw_segment_from_substr);
		}
		
		gc_free(&data->gc, raw_segment_from_substr); // Free the string obtained from gc_substr

		// new_token will gc_strdup final_value_for_new_token
		the_new_token = new_token(&data->gc, final_value_for_new_token, TOKEN_WORD);
		
		// Free final_value_for_new_token as new_token made its own copy
		gc_free(&data->gc, final_value_for_new_token);

		if (the_new_token) {
			add_token(tokens, the_new_token);
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
