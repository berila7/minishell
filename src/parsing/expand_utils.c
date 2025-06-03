/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:59:50 by berila            #+#    #+#             */
/*   Updated: 2025/06/03 18:02:20 by mberila          ###   ########.fr       */
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
