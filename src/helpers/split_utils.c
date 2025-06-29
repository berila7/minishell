/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:45:41 by mberila           #+#    #+#             */
/*   Updated: 2025/06/13 09:53:29 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quote_char(char c, char *in_quote_char)
{
	if (c == '\'' || c == '"')
	{
		if (*in_quote_char == 0)
			*in_quote_char = c;
		else if (*in_quote_char == c)
			*in_quote_char = 0;
	}
}

int	find_word_end(char *s, int start, char *in_quote_char)
{
	int	i;

	i = start;
	*in_quote_char = 0;
	while (s[i])
	{
		handle_quote_char(s[i], in_quote_char);
		if ((s[i] == ' ' || s[i] == '\t') && !*in_quote_char)
			break ;
		i++;
	}
	return (i);
}

int	skip_word_end(char *s, int start, char *in_quote_char)
{
	int	i;

	i = start;
	while (s[i])
	{
		handle_quote_char(s[i], in_quote_char);
		if ((s[i] == ' ' || s[i] == '\t') && !*in_quote_char)
			break ;
		i++;
	}
	return (i);
}
