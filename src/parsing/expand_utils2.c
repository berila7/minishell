/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:02:17 by berila            #+#    #+#             */
/*   Updated: 2025/05/25 15:05:49 by berila           ###   ########.fr       */
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
	if (!expanded || (ft_strlen(expanded) == 0 && !has_quotes))
	{
		gc_free(gc, expanded);
		return;
	}
	if (has_quotes || !data->is_export)
		process_quoted_token(gc, expanded, current_cmd);
	else
		process_unquoted_token(gc, expanded, current_cmd);
	gc_free(gc, expanded);
}
