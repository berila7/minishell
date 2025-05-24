/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:59:50 by berila            #+#    #+#             */
/*   Updated: 2025/05/24 16:09:17 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quotes(char *str, t_expand *exp, t_data *data)
{
	if (str[exp->i] == '\'' && !exp->in_double_quote)
	{
		exp->in_single_quote = !exp->in_single_quote;
		exp->result = ft_strjoin_char_free(&data->gc, exp->result, str[exp->i]);
		exp->i++;
	}
	else if (str[exp->i] == '\"' && !exp->in_single_quote)
	{
		exp->in_double_quote = !exp->in_double_quote;
		exp->result = ft_strjoin_char_free(&data->gc, exp->result, str[exp->i]);
		exp->i++;
	}
}

void	init_expand_vars(t_expand *exp)
{
	exp->in_single_quote = 0;
	exp->in_double_quote = 0;
	exp->i = 0;
}
