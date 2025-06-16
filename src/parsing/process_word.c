/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:14:32 by mberila           #+#    #+#             */
/*   Updated: 2025/06/16 16:29:31 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char	*append_to_result(t_gcnode **gc, char *result, char *new_part)
// {
// 	char	*updated;

// 	if (!new_part)
// 		return (result);
// 	updated = gc_strjoin(gc, result, new_part);
// 	if (updated)
// 		return (updated);
// 	return (result);
// }

// static char	*process_quoted(t_data *data, char *input, int *pos, int len)
// {
// 	char	*result;
// 	int		quote_start;
// 	char	quote;
// 	int		quoted_len;

// 	quote = input[*pos];
// 	quote_start = *pos;
// 	(*pos)++;
// 	while (*pos < len && input[*pos] != quote)
// 		(*pos)++;
// 	quoted_len = *pos - quote_start + 1;
// 	result = gc_substr(&data->gc, input, quote_start, quoted_len);
// 	if (!result)
// 		return (NULL);
// 	(*pos)++;
// 	if (quote == '"')
// 		result = expand_variables(data, result);
// 	char *final_result = remove_quotes(&data->gc, result);
// 	printf("final_result: [%s]\n", final_result);
// 	// if (equal(result, ""))
// 	// 	return (NULL);
// 	return (final_result);
// }

// static char	*process_unquoted(t_data *data, char *input, int *pos, int len)
// {
// 	int		start;
// 	char	*expanded;
// 	char	*unquoted_part;
// 	int		section_len;

// 	start = *pos;
// 	while (*pos < len && input[*pos] != '\'' && input[*pos] != '"')
// 		(*pos)++;
// 	section_len = *pos - start;
// 	if (section_len <= 0)
// 		return (NULL);
// 	unquoted_part = gc_substr(&data->gc, input, start, section_len);
// 	printf("Unquoted part: [%s]\n", unquoted_part);
// 	if (!unquoted_part)
// 		return (NULL);
// 	expanded = expand_variables(data, unquoted_part);
// 	return (expanded);
// }

// char	*process_mixed_quoted(t_data *data, char *input)
// {
// 	char	*result;
// 	int		pos;
// 	int		len;
// 	char	*processed_part;

// 	if (!input)
// 		return (NULL);
// 	len = ft_strlen(input);
// 	pos = 0;
// 	result = gc_strdup(&data->gc, "");
// 	if (!result)
// 		return (NULL);
// 	while (pos < len)
// 	{
// 		processed_part = NULL;
// 		if (input[pos] == '\'' || input[pos] == '"')
// 			processed_part = process_quoted(data, input, &pos, len);
// 		else
// 			processed_part = process_unquoted(data, input, &pos, len);
// 		if (processed_part)
// 			result = append_to_result(&data->gc, result, processed_part);
// 		printf("result: [%s]\n", result);
// 	}
// 	return (result);
// }
