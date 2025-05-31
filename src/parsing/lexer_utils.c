/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:17:51 by berila            #+#    #+#             */
/*   Updated: 2025/05/31 20:02:31 by berila           ###   ########.fr       */
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

char *normalize_word_segment(t_gcnode **gc, const char *segment)
{
    if (!segment)
        return gc_strdup(gc, "");

    int input_len = ft_strlen(segment);
    // Allocate a buffer that can hold the maximum possible length (input_len)
    // This buffer will be freed after creating an exact-size final string.
    char *result_buffer = gc_malloc(gc, input_len + 1);

    int j = 0; // Index for result_buffer
    int i = 0; // Index for segment

    while (i < input_len) {
        if (segment[i] == '"') { // Start of a double-quoted segment or empty double quotes
            if (i + 1 < input_len && segment[i+1] == '"') { // Check for empty ""
                i += 2; // Skip ""
            } else {
                i++; // Skip opening "
                while (i < input_len && segment[i] != '"') {
                    result_buffer[j++] = segment[i++];
                }
                if (i < input_len && segment[i] == '"') {
                    i++; // Skip closing "
                }
                // Note: Unclosed quotes within the segment are assumed to be handled
                // by the earlier `toggel_quote` and `handle_quote_error` logic,
                // which defines the boundaries of `segment`.
            }
        } else if (segment[i] == '\'') { // Start of a single-quoted segment or empty single quotes
            if (i + 1 < input_len && segment[i+1] == '\'') { // Check for empty ''
                i += 2; // Skip ''
            } else {
                i++; // Skip opening '
                while (i < input_len && segment[i] != '\'') {
                    result_buffer[j++] = segment[i++];
                }
                if (i < input_len && segment[i] == '\'') {
                    i++; // Skip closing '
                }
            }
        } else { // Regular, non-quoted character
            result_buffer[j++] = segment[i++];
        }
    }
    result_buffer[j] = '\0'; // Null-terminate the content in the buffer

    // Create a final string of the exact required length
    char *final_str = gc_strdup(gc, result_buffer);
    gc_free(gc, result_buffer); // Free the potentially oversized intermediate buffer

    return final_str;
}
