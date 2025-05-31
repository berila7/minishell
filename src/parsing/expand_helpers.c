/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:11:46 by berila            #+#    #+#             */
/*   Updated: 2025/05/31 18:31:11 by berila           ###   ########.fr       */
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

int	process_var_value(t_gcnode **gc, char *var_name, int i, t_expand *exp)
{
	char	*var_value;

	var_value = get_env(exp->data->env, var_name);
	if (var_value)
		exp->result = ft_strjoin_free(gc, exp->result, var_value);
	printf("Variable: %s, Value: %s\n", var_name, var_value);
	gc_free(gc, var_name);
	return (i);
}
