/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:07:55 by anachat           #+#    #+#             */
/*   Updated: 2025/06/11 15:24:23 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	gc_word_count_quoted(char *s)
{
	int		count;
	int		i;
	char	in_quote_char;

	count = 0;
	i = 0;
	in_quote_char = 0;
	while (s && s[i])
	{
		while (s[i] && (s[i] == ' ' || s[i] == '\t') && !in_quote_char)
			i++;
		if (s[i])
		{
			count++;
			i = skip_word_end(s, i, &in_quote_char);
		}
	}
	return (count);
}

static char	**gc_init_split_quoted(t_gcnode **gc, char *s, char **result_array)
{
	int		i;
	int		j;
	int		word_start;
	char	in_quote_char;

	i = 0;
	j = 0;
	while (s && s[i])
	{
		in_quote_char = 0;
		while (s[i] && (s[i] == ' ' || s[i] == '\t'))
			i++;
		if (s[i])
		{
			word_start = i;
			i = find_word_end(s, word_start, &in_quote_char);
			result_array[j++] = gc_substr(gc, s, word_start, i - word_start);
		}
	}
	return (result_array);
}

char	**gc_split(t_gcnode **gc, char *s)
{
	char	**res;
	int		num_words;

	if (!s)
		return (NULL);
	num_words = gc_word_count_quoted(s);
	res = gc_malloc(gc, (num_words + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	return (gc_init_split_quoted(gc, s, res));
}
