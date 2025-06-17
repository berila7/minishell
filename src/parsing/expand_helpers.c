/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:11:46 by berila            #+#    #+#             */
/*   Updated: 2025/06/17 10:47:21 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_named_var(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	int		start;
	char	*var_name;

	start = i;
	i = get_var_name_end(str, i);
	var_name = gc_substr(gc, str, start, i - start);
	return (process_var_value(gc, var_name, i, exp));
}

int	get_var_name_end(char *str, int i)
{
	while (str[i] && (is_valid_var_char(str[i])))
		i++;
	return (i);
}

char	*replace_quotes(t_gcnode **gc, char *str)
{
	char	*result;
	int		i;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = gc_malloc(gc, len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (str[i] == '\'')
			result[i] = SINGLE_QUOTE;
		else if (str[i] == '\"')
			result[i] = DOUBLE_QUOTE;
		else
			result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

int	process_var_value(t_gcnode **gc, char *var_name, int i, t_expand *exp)
{
	char	*var_value;
	char	*temp;

	var_value = get_env(exp->data->env, var_name);
	if (var_value)
	{
		temp = replace_quotes(gc, var_value);
		if (temp)
			exp->result = ft_strjoin_free(gc, exp->result, temp);
	}
	gc_free(gc, var_name);
	return (i);
}
