/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:25:34 by berila            #+#    #+#             */
/*   Updated: 2025/06/02 11:21:38 by mberila          ###   ########.fr       */
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
	t_token *current;

	(void)data;
	current = (*tokens);
	while (current)
	{
		key = token_key(data, current->value);
		value = token_value(data, current->value);
		if (data->regular_export && equal(current->value, "export") && !current->prev)
			data->is_export = 0;
		if ((ft_strchr(key, '$') && ft_strchr(value, '$')))
			data->is_export = 1;
		current = current->next;
	}
}
