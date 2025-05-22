/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:59:50 by berila            #+#    #+#             */
/*   Updated: 2025/05/22 15:06:23 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_quote(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	exp->in_single_quote = !(exp->in_single_quote);
	exp->result = ft_strjoin_char_free(gc, exp->result, str[i]);
	return (i + 1);
}

int	handle_double_quote(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	exp->in_double_quote = !(exp->in_double_quote);
	exp->result = ft_strjoin_char_free(gc, exp->result, str[i]);
	return (i + 1);
}

int	handle_regular_char(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	exp->result = ft_strjoin_char_free(gc, exp->result, str[i]);
	return (i + 1);
}

int	handle_variable(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	i++;
	if (str[i] == '?')
		return (handle_exit_status(gc, i, exp));
	else if (ft_isdigit(str[i]))
		return (handle_digit_var(gc, str, i, exp));
	else if (str[i] && is_valid_var_char(str[i]))
		return (handle_named_var(gc, str, i, exp));
	else
	{
		exp->result = ft_strjoin_char_free(gc, exp->result, '$');
		return (i);
	}
}

int	process_char(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	if (str[i] == '\'' && !exp->in_double_quote)
		return (handle_single_quote(gc, str, i, exp));
	else if (str[i] == '\"' && !exp->in_single_quote)
		return (handle_double_quote(gc, str, i, exp));
	else if (str[i] == '$' && str[i + 1]
		&& (!exp->in_single_quote || exp->data->in_heredoc))
		return (handle_variable(gc, str, i, exp));
	else
		return (handle_regular_char(gc, str, i, exp));
}
