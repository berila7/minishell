/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:25:34 by berila            #+#    #+#             */
/*   Updated: 2025/06/01 16:34:44 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*token_key(t_data *data, char *str)
{
	char	*key;
	int		i;

	if (!str || !*str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=' && str[i] != ' ')
		i++;
	key = gc_substr(&data->gc, str, 0, i);
	if (!key)
		return (NULL);
	return (key);
}

char	*token_value(t_data *data, char *str)
{
	char	*value;
	int		i;

	if (!str || !*str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		i++;
	value = gc_substr(&data->gc, str, i, ft_strlen(str) - i);
	if (!value)
		return (NULL);
	return (value);
}

void	export_handler(t_token **tokens, t_data *data)
{
	char	*key;
	char	*value;

	if ((equal((*tokens)->value, "export"))
		|| ((*tokens)->type == TOKEN_REDIR_APPEND
			|| (*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT)
	)
	{
		printf("Im the first who sets is_export to 0\n");
		data->is_export = 0;
	}
	if ((*tokens)->next && (*tokens)->next->value)
	{
		key = token_key(data, (*tokens)->next->value);
		value = token_value(data, (*tokens)->next->value);
	}
	if ((*tokens)->next
		&& (*tokens)->next->value && ft_strchr(key, '$')
		&& ft_strchr(value, '$')
	)
	{
		printf("Im the middle who sets is_export to 1\n");
		data->is_export = 1;
	}
	if ((*tokens)->next && (*tokens)->next->next
		&& !equal((*tokens)->next->next->value, "export"))
	{
		printf("Im the last who sets is_export to 1\n");
		data->is_export = 1;
	}
}
