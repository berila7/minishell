/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:50:25 by berila            #+#    #+#             */
/*   Updated: 2025/06/09 19:59:07 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/// neeeeeed fiiiiix heeeereeeeee

int	process_char(t_data *data, char *str, int i, t_expand *exp)
{
	if (str[i] == '\'' && !exp->in_double_quote)
		return (handle_single_quote(&data->gc, str, i, exp));
	else if (str[i] == '\"' && !exp->in_single_quote)
		return (handle_double_quote(&data->gc, str, i, exp));
	else if (str[i] == '$' && str[i + 1]
		&& (!exp->in_single_quote || exp->data->in_heredoc))
	{
		if (data->expandable)
			return (handle_variable(&data->gc, str, i, exp));
		else
			return (handle_regular_char(&data->gc, str, i, exp));
	}
	else
		return (handle_regular_char(&data->gc, str, i, exp));
}

char	*expand_variables(t_data *data, char *str)
{
	t_expand	exp;
	int			i;

	exp.result = gc_strdup(&data->gc, "");
	if (!exp.result)
		return (NULL);
	exp.in_single_quote = 0;
	exp.in_double_quote = 0;
	exp.data = data;
	i = 0;
	while (str[i])
	{
		i = process_char(data, str, i, &exp);
		if (i == -1)
			return (NULL);
	}
	return (exp.result);
}
