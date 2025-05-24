/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:02:17 by berila            #+#    #+#             */
/*   Updated: 2025/05/24 15:34:26 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_token_word(t_gcnode **gc, t_token *token,
	t_cmd *current_cmd, t_data *data)
{
	char	*expanded;
	int		has_quotes;

	expanded = expand_variables(gc, token->value, data);
	has_quotes = in_quotes(token->value);
	if (has_quotes || !data->is_export)
		process_quoted_token(gc, expanded, current_cmd);
	else
		process_unquoted_token(gc, expanded, current_cmd);
	gc_free(gc, expanded);
}

char	*word_split_join(t_gcnode **gc, char *dest, char *src)
{
	int		i;
	int		was_space;
	char	*result;

	i = 0;
	was_space = 1;
	result = dest;
	while (src[i])
	{
		if (src[i] == ' ' || src[i] == '\t')
		{
			if (!was_space)
			{
				result = ft_strjoin_char_free(gc, result, ' ');
				was_space = 1;
			}
			while (src[i] && (src[i] == ' ' || src[i] == '\t'))
				i++;
		}
		else
		{
			result = ft_strjoin_char_free(gc, result, src[i]);
			was_space = 0;
			i++;
		}
	}
	return (result);
}
