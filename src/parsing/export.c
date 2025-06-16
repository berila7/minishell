/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:25:34 by berila            #+#    #+#             */
/*   Updated: 2025/06/16 15:09:25 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_export(t_token *current, t_data *data)
{
	char	*key;
	char	*value;
	char	*result;

	key = token_key(data, current->value);
	value = token_value(data, current->value);
	if (!ft_strchr(key, '$') && ft_strchr(value, '$') && current->quote_type == 0)
	{
		current->quote_type = 2;
		value = add_quotes_to_str(&data->gc, value);
		key = ft_strjoin_char_free(&data->gc, key, '=');
		result = gc_strjoin(&data->gc, key, value);
		current->value = result;
	}
}

void	export_handler(t_token **tokens, t_data *data)
{
	t_token	*current;

	current = (*tokens);
	export_exist(tokens, data);
	if (equal(current->value, "export")
		&& !current->prev && current->next
	)
	{
		current = (*tokens)->next;
		while (current)
		{
			process_export(current, data);
			current = current->next;
		}
	}
}

void	export_exist(t_token **tokens, t_data *data)
{
	t_token	*current;
	char	*unquoted_str;

	current = (*tokens);
	while (current)
	{
		unquoted_str = remove_quotes(&data->gc, current->value);
		if (equal(unquoted_str, "export"))
			data->is_export = 1;
		current = current->next;
	}
}
