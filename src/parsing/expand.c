/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:33:56 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 19:43:01 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_expand_vars(t_expand *exp)
{
	exp->in_single_quote = 0;
	exp->in_double_quote = 0;
	exp->i = 0;
}

static void	handle_quotes(char *str, t_expand *exp)
{
	if (str[exp->i] == '\'' && !exp->in_double_quote)
	{
		exp->in_single_quote = !exp->in_single_quote;
		exp->result = ft_strjoin_char_free(exp->result, str[exp->i]);
		exp->i++;
	}
	else if (str[exp->i] == '\"' && !exp->in_single_quote)
	{
		exp->in_double_quote = !exp->in_double_quote;
		exp->result = ft_strjoin_char_free(exp->result, str[exp->i]);
		exp->i++;
	}
}

static void	handle_special_var(char *str, t_expand *exp, t_data *data)
{
	char	*status_str;
	char	first_digit;

	if (str[exp->i] == '?')
	{
		status_str = ft_itoa(data->exit_status);
		exp->result = ft_strjoin_free(exp->result, status_str);
		free(status_str);
		exp->i++;
	}
	else if (ft_isdigit(str[exp->i]))
	{
		first_digit = str[exp->i];
		exp->i++;
		if (first_digit == '0')
			exp->result = ft_strjoin_free(exp->result, "minishell");
		// Don't need to handle other numeric parameters since they're not set in your shell
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
	var_name = ft_substr(str, start, exp->i - start);
	var_value = get_env(data->env, var_name);
	free(var_name);
	if (var_value)
	{
		if (exp->in_double_quote)
			exp->result = ft_strjoin_free(exp->result, var_value);
		else
			exp->result = word_split_join(exp->result, var_value);
	}
}

static void	process_dollar(char *str, t_expand *exp, t_data *data)
{
	exp->i++;
	if (str[exp->i] == '?' || ft_isdigit(str[exp->i]))
		handle_special_var(str, exp, data);
	else if (str[exp->i] && is_valid_var_char(str[exp->i]))
		expand_env_var(str, exp, data);
	else
		exp->result = ft_strjoin_char_free(exp->result, '$');
}

char	*expand_variables(char *str, t_data *data)
{
	t_expand	exp;

	exp.result = ft_strdup("");
	if (!exp.result)
		return (NULL);
	init_expand_vars(&exp);
	while (str[exp.i])
	{
		if (str[exp.i] == '\'' || str[exp.i] == '\"')
			handle_quotes(str, &exp);
		else if (str[exp.i] == '$' && str[exp.i + 1]
			&& (!exp.in_single_quote || data->in_heredoc))
			process_dollar(str, &exp, data);
		else
		{
			exp.result = ft_strjoin_char_free(exp.result, str[exp.i]);
			exp.i++;
		}
	}
	return (exp.result);
}
