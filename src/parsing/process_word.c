/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:14:32 by mberila           #+#    #+#             */
/*   Updated: 2025/06/04 14:34:03 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *append_to_result(t_gcnode **gc, char *result, char *new_part) {
	char *updated;

	if (!new_part)
		return (result);	
	updated = gc_strjoin(gc, result, new_part);
	if (updated)
		return (updated);
	return (result);
}

static char *extract_substring(t_gcnode **gc, const char *input, int start, int len) {
	char *substr;

	substr = gc_malloc(gc, len + 1);
	if (!substr) 
		return (NULL);
	strncpy(substr, input + start, len);
	substr[len] = '\0';
	return (substr);
}

static char *process_quoted_section(t_data *data, char *input, int *pos, int len) 
{
	char	*remaining;
	char	*quoted_part;
	int		quote_start;
	char	quote;
	int		quoted_len;
	char	*expanded;

	quote = input[*pos];
	quote_start = *pos;
	(*pos)++; // Skip opening quote

	// Find closing quote
	while (*pos < len && input[*pos] != quote)
		(*pos)++;
	// Handle unclosed quote - expand remaining as-is
	if (*pos >= len)
	{
		remaining = extract_substring(&data->gc, input, quote_start, len - quote_start);
		if (!remaining) 
			return NULL;
		return expand_variables(data, remaining); // Note: data parameter missing in original
	}
	quoted_len = *pos - quote_start + 1;
	quoted_part = extract_substring(&data->gc, input, quote_start, quoted_len);
	if (!quoted_part) 
		return NULL;
	(*pos)++; // Skip closing quote
	printf("Im the quoted Part: [%s]\n", quoted_part);
	expanded = expand_variables(data, quoted_part);
	return remove_quotes(&data->gc, expanded);
}

static char *process_unquoted_section(t_data *data, char *input, int *pos, int len)
{
	int		start;
	char	*expanded;
	char	*unquoted_part;
	int		section_len;

	start = *pos;
	// Find next quote or end of string
	while (*pos < len && input[*pos] != '\'' && input[*pos] != '"')
		(*pos)++;	
	section_len = *pos - start;
	if (section_len <= 0)
		return NULL;
	unquoted_part = extract_substring(&data->gc, input, start, section_len);
	if (!unquoted_part) 
		return NULL;
	expanded = expand_variables(data, unquoted_part);
	printf("Im the unquoted Part: [%s]\n", expanded);
	return (expanded);
}

char *process_mixed_quoted(t_data *data, char *input)
{
	char *result;
	int pos;
	int len;

	if (!input) return NULL;
	len = ft_strlen(input);
	pos = 0;
	result = gc_strdup(&data->gc, "");
	if (!result) 
		return NULL;
	while (pos < len) {
		char *processed_part = NULL;
		
		if (input[pos] == '\'' || input[pos] == '"')
			processed_part = process_quoted_section(data, input, &pos, len);
		else
			processed_part = process_unquoted_section(data, input, &pos, len);
		if (processed_part)
			result = append_to_result(&data->gc, result, processed_part);
	}
	
	return result;
}
