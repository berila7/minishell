/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:25:34 by berila            #+#    #+#             */
/*   Updated: 2025/06/03 13:09:04 by berila           ###   ########.fr       */
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

char	*add_quotes_to_str(t_gcnode **gc, char *str)
{
	char	*quoted_str;
	int		len;

	len = ft_strlen(str);
	if (!str || !*str)
		return (NULL);
	quoted_str = gc_malloc(gc, len + 3);
	int	i = 0;
	int	j = 0;
	quoted_str[0] = '\"';
	j += 1;
	while (str[i])
		quoted_str[j++] = str[i++];
	quoted_str[len + 1] = '\"';
	quoted_str[len + 2] = '\0';
	return (quoted_str);
}

void	export_handler(t_token **tokens, t_data *data)
{
	char	*key;
	char	*value;
	t_token *current;
	char	*key_str;
	char	*new_value;
	char	*result;

	current = (*tokens);
	export_exist(tokens, data);
	if (data->is_export
		&& !current->prev && current->next
	)
	{
		current = (*tokens)->next;
		while (current)
		{
			key = token_key(data, current->value);
			value = token_value(data, current->value);
			if (!ft_strchr(key, '$') && ft_strchr(value, '$'))
			{
				current->quote_type = 2;
				new_value = add_quotes_to_str(&data->gc, value);
				key_str = ft_strjoin_char_free(&data->gc, key, '=');
				result = gc_strjoin(&data->gc, key_str, new_value);
				current->value = result;
				printf("new value: [%s]\n", current->value);
			}
			current = current->next;
		}
	}
}

// void	export_handler(t_token **tokens, t_data *data)
// {
// 	char	*key;
// 	char	*value;
// 	t_token *current;
// 	char	*key_str;
// 	char	*new_value;
// 	char	*result;

// 	current = (*tokens);
// 	while (current)
// 	{
// 		if (equal(current->value, "export")
// 			&& !current->prev && current->next
// 			&& data->regular_export
// 		)
// 		{
// 			key = token_key(data, current->next->value);
// 			value = token_value(data, current->next->value);
// 			if (!ft_strchr(key, '$') && ft_strchr(value, '$'))
// 			{
// 				data->is_export = 1;
// 				current->next->quote_type = 2;
// 				new_value = add_quotes_to_str(&data->gc, value);
// 				key_str = ft_strjoin_char_free(&data->gc, key, '=');
// 				result = gc_strjoin(&data->gc, key_str, new_value);
// 				current->next->value = result;
// 				printf("new value: [%s]\n", current->next->value);
// 			}
// 			else
// 			{
// 				data->is_export = 0;
// 				current->quote_type = 0;
// 			}
// 		}
// 		current = current->next;
// 	}
// }

void	export_exist(t_token **tokens, t_data *data)
{
	t_token	*current;

	current = (*tokens);
	while (current)
	{
		if (equal(current->value, "export"))
			data->is_export = 1;
		current = current->next;
	}
}
