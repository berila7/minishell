/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:00:00 by mberila           #+#    #+#             */
/*   Updated: 2025/06/11 13:31:32 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_quoted_part(char *str)
{
	int		len;
	int		pos;
	char	q;

	len = ft_strlen(str);
	pos = 0;
	while (pos < len)
	{
		if (str[pos] == '\'' || str[pos] == '"')
		{
			q = str[pos];
			pos++;
			while (pos < len && str[pos] != q)
				pos++;
			if (pos < len)
				return (true);
		}
		pos++;
	}
	return (false);
}

bool	has_unquoted_part(char *str)
{
	int		len;
	int		pos;
	char	q;

	len = ft_strlen(str);
	pos = 0;
	while (pos < len)
	{
		if (str[pos] == '\'' || str[pos] == '"')
		{
			q = str[pos];
			pos++;
			while (pos < len && str[pos] != q)
				pos++;
			if (pos < len)
				pos++;
			else
				return (true);
		}
		else
			return (true);
	}
	return (false);
}

bool	has_mixed_format(char *str)
{
	if (!str || ft_strlen(str) == 0)
		return (false);
	return (has_quoted_part(str) && has_unquoted_part(str));
}
