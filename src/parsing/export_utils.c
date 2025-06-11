/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:27:38 by mberila           #+#    #+#             */
/*   Updated: 2025/06/11 14:27:49 by mberila          ###   ########.fr       */
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
	int		i;
	int		j;

	if (!str || !*str)
		return (NULL);
	len = ft_strlen(str);
	quoted_str = gc_malloc(gc, len + 3);
	i = 0;
	j = 0;
	quoted_str[j++] = '\"';
	while (str[i])
		quoted_str[j++] = str[i++];
	quoted_str[j++] = '\"';
	quoted_str[j] = '\0';
	return (quoted_str);
}
