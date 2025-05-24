/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:50:25 by berila            #+#    #+#             */
/*   Updated: 2025/05/24 16:15:10 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_special_var(char *str, t_expand *exp, t_data *data)
{
	char	*status_str;
	char	first_digit;

	if (str[exp->i] == '?')
	{
		status_str = gc_itoa(&data->gc, exit_status(0, 0));
		exp->result = ft_strjoin_free(&data->gc, exp->result, status_str);
		gc_free(&data->gc, status_str);
		exp->i++;
	}
	else if (ft_isdigit(str[exp->i]))
	{
		first_digit = str[exp->i];
		exp->i++;
		if (first_digit == '0')
			exp->result = ft_strjoin_free(&data->gc, exp->result,
					"minishell");
	}
}

static void	expand_env_var(char *str, t_expand *exp, t_data *data)
{
	int		start;
	char	*var_name;
	char	*var_value;

	start = exp->i;
	while (str[exp->i] && (is_valid_var_char(str[exp->i])))
		exp->i++;
	var_name = gc_substr(&data->gc, str, start, exp->i - start);
	var_value = get_env(data->env, var_name);
	printf("var value %s\n", var_value);
	gc_free(&data->gc, var_name);
	if (var_value)
	{
		if (exp->in_double_quote)
			exp->result = ft_strjoin_free(&data->gc, exp->result, var_value);
		else
			exp->result = word_split_join(&data->gc, exp->result, var_value);
	}
	else
		exp->result = NULL;
}

static void	process_dollar(char *str, t_expand *exp, t_data *data)
{
	exp->i++;
	if (str[exp->i] == '?' || ft_isdigit(str[exp->i]))
		handle_special_var(str, exp, data);
	else if (str[exp->i] && is_valid_var_char(str[exp->i]))
		expand_env_var(str, exp, data);
	else
		exp->result = ft_strjoin_char_free(&data->gc, exp->result, '$');
}

char	*expand_variables(t_gcnode **gc, char *str, t_data *data)
{
	t_expand	exp;

	exp.result = gc_strdup(gc, "");
	init_expand_vars(&exp);
	while (str[exp.i])
	{
		if (str[exp.i] == '\'' || str[exp.i] == '\"')
			handle_quotes(str, &exp, data);
		else if (str[exp.i] == '$' && str[exp.i + 1]
			&& (!exp.in_single_quote || data->in_heredoc))
			process_dollar(str, &exp, data);
		else
		{
			exp.result = ft_strjoin_char_free(gc, exp.result, str[exp.i]);
			exp.i++;
		}
	}
	return (exp.result);
}
