/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:02:48 by mberila           #+#    #+#             */
/*   Updated: 2025/06/18 16:11:59 by mberila          ###   ########.fr       */
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
	if (!ft_strchr(key, '$') && ft_strchr(value, '$' && is_valid_env_key(key))
		&& current->quote_type == 0)
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
	int		is_redir;

	current = (*tokens);
	is_redir = 0;
	if ((current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND || current->type == TOKEN_HEREDOC)
		&& current->next && current->next->next)
			is_redir = 1;
	export_exist(tokens, data);
	if (equal(current->value, "export") && current->next
		&& (!current->prev || is_redir)
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
