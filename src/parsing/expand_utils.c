/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:59:50 by berila            #+#    #+#             */
/*   Updated: 2025/06/16 10:25:16 by mberila          ###   ########.fr       */
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

// int	handle_digit_var(t_gcnode **gc, char *str, int i, t_expand *exp)
// {
// 	char	first_digit;

// 	first_digit = str[i++];
// 	return (i);
// }

int	is_ambiguous(t_token *token, char *str)
{
	int	i;

	if (!str || !str[0])
		return (1);
	i = 0;
	if (!equal(token->value, str))
	{
		while (str[i])
		{
			if (str[i] == ' ' || str[i] == '\t')
				return (1);
			i++;
		}
	}
	return (0);
}
