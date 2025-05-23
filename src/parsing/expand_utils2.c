/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:02:17 by berila            #+#    #+#             */
/*   Updated: 2025/05/23 11:08:30 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exit_status(t_gcnode **gc, int i, t_expand *exp)
{
	char	*status_str;

	status_str = gc_itoa(gc, exit_status(0, 0));
	exp->result = ft_strjoin_free(gc, exp->result, status_str);
	gc_free(gc, status_str);
	return (i + 1);
}

int	handle_digit_var(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	char	first_digit;

	first_digit = str[i++];
	if (first_digit == '0')
		exp->result = ft_strjoin_free(gc, exp->result, "minishell");
	return (i);
}

void	process_token_word(t_gcnode **gc, t_token *token,
	t_cmd *current_cmd, t_data *data)
{
	char	*expanded;
	int		has_quotes;

	expanded = expand_variables(gc, token->value, data);
	has_quotes = check_quotes_in_token(token->value);
	if (has_quotes || !data->is_export)
		process_quoted_token(gc, expanded, current_cmd);
	else
		process_unquoted_token(gc, expanded, current_cmd);
	gc_free(gc, expanded);
}
