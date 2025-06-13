/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:14:32 by mberila           #+#    #+#             */
/*   Updated: 2025/06/13 11:33:03 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_to_result(t_gcnode **gc, char *result, char *new_part)
{
	char	*updated;

	if (!new_part)
		return (result);
	updated = gc_strjoin(gc, result, new_part);
	if (updated)
		return (updated);
	return (result);
}

static char	*extract_substring(t_gcnode **gc, char *input,
	int start, int len)
{
	char	*substr;

	substr = gc_malloc(gc, len + 1);
	if (!substr)
		return (NULL);
	ft_strncpy(substr, input + start, len);
	substr[len] = '\0';
	return (substr);
}

static char	*process_quoted(t_data *data, char *input, int *pos, int len)
{
	char	*result;
	int		quote_start;
	char	quote;
	int		quoted_len;

	quote = input[*pos];
	quote_start = *pos;
	(*pos)++;
	while (*pos < len && input[*pos] != quote)
		(*pos)++;
	if (*pos >= len)
	{
		result = extract_substring(&data->gc, input,
				quote_start, len - quote_start);
		if (!result)
			return (NULL);
		return (expand_variables(data, result));
	}
	quoted_len = *pos - quote_start + 1;
	result = extract_substring(&data->gc, input, quote_start, quoted_len);
	if (!result)
		return (NULL);
	(*pos)++;
	result = expand_variables(data, result);
	return (remove_quotes(&data->gc, result));
}

static char	*process_unquoted(t_data *data, char *input, int *pos, int len)
{
	int		start;
	char	*expanded;
	char	*unquoted_part;
	int		section_len;

	start = *pos;
	while (*pos < len && input[*pos] != '\'' && input[*pos] != '"')
		(*pos)++;
	section_len = *pos - start;
	if (section_len <= 0)
		return (NULL);
	unquoted_part = extract_substring(&data->gc, input, start, section_len);
	if (!unquoted_part)
		return (NULL);
	expanded = expand_variables(data, unquoted_part);
	return (expanded);
}

char	*process_mixed_quoted(t_data *data, char *input)
{
	char	*result;
	int		pos;
	int		len;
	char	*processed_part;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	pos = 0;
	result = gc_strdup(&data->gc, "");
	if (!result)
		return (NULL);
	while (pos < len)
	{
		processed_part = NULL;
		if (input[pos] == '\'' || input[pos] == '"')
			processed_part = process_quoted(data, input, &pos, len);
		else
			processed_part = process_unquoted(data, input, &pos, len);
		if (processed_part)
			result = append_to_result(&data->gc, result, processed_part);
	}
	return (result);
}
