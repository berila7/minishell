/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:50:25 by berila            #+#    #+#             */
/*   Updated: 2025/05/31 16:47:58 by berila           ###   ########.fr       */
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

int	process_char(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	if (str[i] == '\'' && !exp->in_double_quote)
	{
		// If not already inside single quotes and we encounter a literal '' pair
		if (!exp->in_single_quote && str[i + 1] == '\'')
		{
			return (i + 2); // Consume both ' characters, append nothing, advance index by 2
		}
		// Otherwise, handle single quote normally (it's part of a larger quoted sequence or a lone quote)
		return (handle_single_quote(gc, str, i, exp));
	}
	else if (str[i] == '\"' && !exp->in_single_quote)
	{
		// If not already inside double quotes and we encounter a literal "" pair
		if (!exp->in_double_quote && str[i + 1] == '\"')
		{
			return (i + 2); // Consume both " characters, append nothing, advance index by 2
		}
		// Otherwise, handle double quote normally
		return (handle_double_quote(gc, str, i, exp));
	}
	else if (str[i] == '$' && str[i + 1]
		&& (!exp->in_single_quote || exp->data->in_heredoc))
		return (handle_variable(gc, str, i, exp));
	else
		return (handle_regular_char(gc, str, i, exp));
}

char	*expand_variables(t_gcnode **gc, char *str, t_data *data)
{
	t_expand	exp;
	int			i;

	exp.result = gc_strdup(gc, "");
	if (!exp.result)
		return (NULL);
	exp.in_single_quote = 0;
	exp.in_double_quote = 0;
	exp.data = data;
	i = 0;
	while (str[i])
	{
		i = process_char(gc, str, i, &exp);
		if (i == -1)
			return (NULL);
	}
	return (exp.result);
}
