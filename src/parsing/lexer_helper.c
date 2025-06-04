/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:00:00 by mberila           #+#    #+#             */
/*   Updated: 2025/06/04 12:01:22 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool has_quoted_part(char *str)
{
	int len = ft_strlen(str);
	int pos = 0;
	while (pos < len)
	{
		if (str[pos] == '\'' || str[pos] == '"')
        {
            char q = str[pos];
			pos++;
			while (pos < len && str[pos] != q)
				pos++;
			if (pos < len)
				return (true);
		}
		pos++;
	}
	return (false);
}

bool has_unquoted_part(char *str)
{
	int	len;
	int	pos;

	len = ft_strlen(str);
	pos = 0;
	while (pos < len)
	{
		if (str[pos] == '\'' || str[pos] == '"')
        {
            char q = str[pos];
			pos++;
			while (pos < len && str[pos] != q)
				pos++;
			if (pos < len)
				pos++;
			else
				return (true);
		}
		else
		{
			return (true);
		}
	}
	return (false);
}

bool has_mixed_format(char *str)
{
	if (!str || ft_strlen(str) == 0)
		return (false);
	return (has_quoted_part(str) && has_unquoted_part(str));
}

// char *process_mixed_quoted(t_gcnode **gc, char *input, t_data *data)
// {
// 	if (!input)
// 		return NULL;
// 	int len = strlen(input);
// 	int pos = 0;
// 	char *result = gc_strdup(gc, "");
	
// 	if (!result)
// 		return NULL;
	
// 	while (pos < len)
// 	{
// 		if (input[pos] == '"')
// 		{
// 			int quote_start = pos;
// 			pos++;
// 			while (pos < len && input[pos] != '"')
// 				pos++;
// 			if (pos >= len)
// 			{
// 				char *remaining = gc_malloc(gc, len - quote_start + 1);
// 				if (!remaining)
// 					return result;
// 				strcpy(remaining, input + quote_start);
// 				char *expanded = expand_variables(gc, remaining, data);
// 				if (!expanded)
// 					return result;
// 				char *new_result = gc_strjoin(gc, result, expanded);
// 				return new_result ? new_result : result;
// 			}
// 			int quoted_len = pos - quote_start + 1;
// 			char *quoted_part = gc_malloc(gc, quoted_len + 1);
// 			if (!quoted_part)
// 				return result;
// 			strncpy(quoted_part, input + quote_start, quoted_len);
// 			quoted_part[quoted_len] = '\0';
// 			char *unquoted = remove_quotes(gc, quoted_part);
// 			if (!unquoted)
// 				return result;
// 			char *new_result = gc_strjoin(gc, result, unquoted);
// 			if (!new_result)
// 				return result;
// 			result = new_result;
// 			pos++;
// 		}
// 		else
// 		{
// 			int start = pos;
// 			while (pos < len && input[pos] != '"')
// 				pos++;
// 			int unquoted_len = pos - start;
// 			if (unquoted_len > 0)
// 			{
// 				char *unquoted_part = gc_malloc(gc, unquoted_len + 1);
// 				if (!unquoted_part)
// 					return result;
// 				strncpy(unquoted_part, input + start, unquoted_len);
// 				unquoted_part[unquoted_len] = '\0';
// 				char *expanded = expand_variables(gc, unquoted_part, data);
// 				if (!expanded)
// 					return result;
// 				char *new_result = gc_strjoin(gc, result, expanded);
// 				if (!new_result)
// 					return result;
// 				result = new_result;
// 			}
// 		}
// 	}
// 	return result;
// }
