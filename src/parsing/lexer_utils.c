/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:17:51 by berila            #+#    #+#             */
/*   Updated: 2025/06/02 11:20:59 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_quotes(t_token **token)
{
	int	i = 0;
	int	quotes_type;

	quotes_type = 0;
	(*token)->quote_type = 0;
	(*token)->expanded = 1;
	(*token)->splited = 1;
	while ((*token)->value[i])
	{
		if ((*token)->value[i] == '\'')
		{
			if (quotes_type == 0)
				quotes_type = 1;
			else if (quotes_type == 1)
			{
				quotes_type = 0;
				(*token)->quote_type = 1;
				(*token)->expanded = 0;
				(*token)->splited = 0;
			}
		}
		else if ((*token)->value[i] == '\"')
		{
			if (quotes_type == 0)
				quotes_type = 2;
			else if (quotes_type == 2)
			{
				quotes_type = 0;
				(*token)->quote_type = 2;
				if (ft_strchr((*token)->value, '$'))
					(*token)->splited = 1;
				else
					(*token)->splited = 0;
			}
		}
		i++;
	}
	
}

t_token	*new_token(t_gcnode **gc, char *value, t_token_type type)
{
	t_token	*token;

	token = gc_malloc(gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = gc_strdup(gc, value);
	if (!token->value)
	{
		gc_free(gc, token);
		return (NULL);
	}
	token->type = type;
	check_quotes(&token);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		new_token->prev = NULL;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
	new_token->next = NULL;
}

int	toggel_quote(char *line, int *i)
{
	int	in_quote;

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
		if (!in_quote && (line[*i] == ' ' || line[*i] == '\t'
				|| line[*i] == '|' || line[*i] == '<' || line[*i] == '>'))
			break ;
		(*i)++;
	}
	return (in_quote);
}

int	handle_quote_error(t_token **tokens, t_data *data, int in_quote)
{
	char	*quote_state;

	if (in_quote == 0)
		return (0);
	if (in_quote == 1)
		quote_state = "single";
	else
		quote_state = "double";
	printf("minishell: syntax error: unclosed %s quote\n",
		quote_state);
	free_tokens(&data->gc, *tokens);
	*tokens = NULL;
	return (1);
}

bool is_simple_non_empty_quoted_string(char *s) {
    if (!s) return false;
    int len = ft_strlen(s);

    // Must be at least 2 characters long (e.g., "" or '')
    if (len < 2) return false;

    char quote_char = s[0];
    // Must start with ' or "
    if (quote_char != '"' && quote_char != '\'') {
        return false;
    }

    // Must end with the same quote char
    if (s[len-1] != quote_char) {
        return false;
    }

    // Check the content between the outer quotes (s[1] to s[len-2])
    // It should not contain the *same* outer quote character unescaped.
    // For minishell's typical word tokenization, we usually don't deal with complex escapes
    // like \" inside "...", so finding the quote_char is enough to mark it as not "simple content".
	int k = 1;
    while (k <= len - 2) 
	{
        if (s[k] == quote_char) 
		{
            return false; // Found the same quote character inside, indicating a more complex structure
                          // e.g., "foo"bar" (if `toggel_quote` grouped it) or "a"b"
        }
		k++;
    }
    
    // If all checks pass, it's a simple quoted string like "content" or 'content'
    // This also includes "" and '' as simple quoted strings.
    return true;
}

char *process_complex_segment(t_data *data, char *segment)
{

    if (!segment) 
	{
        return gc_strdup(&data->gc, "");
    }

    int input_len = ft_strlen(segment);
    if (input_len == 0) 
	{
        return gc_strdup(&data->gc, "");
    }
    // Handle the case where the entire segment is just "" or '' specifically first
    // as they should result in an empty string.
    if (input_len == 2) 
	{
        if ((segment[0] == '"' && segment[1] == '"') || \
            (segment[0] == '\'' && segment[1] == '\'')) 
		{
            return gc_strdup(&data->gc, "");
        }
    }

    // Max possible length, will be realloc'd to exact size via gc_strdup at the end
    char *result_buffer = gc_malloc(&data->gc, input_len + 1); 
    int j = 0; // Index for result_buffer
    int i = 0; // Index for segment

    while (i < input_len)
	{
        if (segment[i] == '"')
		{
            if (i + 1 < input_len && segment[i+1] == '"')
			{ // Empty ""
                i += 2; // Skip
            } 
			else 
			{ // Non-empty "..."
                i++; // Skip opening "
                while (i < input_len && segment[i] != '"') 
				{
                    result_buffer[j++] = segment[i++];
                }
                if (i < input_len && segment[i] == '"') 
				{
                    i++; // Skip closing "
                }
            }
        }
		else if (segment[i] == '\'') 
		{
            if (i + 1 < input_len && segment[i+1] == '\'') 
			{ // Empty ''
                i += 2; // Skip
            } 
			else 
			{ // Non-empty '...'
                i++; // Skip opening '
                while (i < input_len && segment[i] != '\'') 
				{
                    result_buffer[j++] = segment[i++];
                }
                if (i < input_len && segment[i] == '\'') 
				{
                    i++; // Skip closing '
                }
            }
        } 
		else 
		{ // Regular, non-quoted character
            result_buffer[j++] = segment[i++];
        }
    }
    result_buffer[j] = '\0';

    char *final_str = gc_strdup(&data->gc, result_buffer); // gc_strdup creates an exact size copy
    gc_free(&data->gc, result_buffer); // Free the potentially oversized buffer
	if (equal(final_str, "export") && !equal(final_str, segment))
		data->regular_export = 0;
	else if (equal(final_str, "export") && equal(final_str, segment))
		data->regular_export = 1;
    return final_str;
}
