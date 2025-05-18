/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:07:55 by anachat           #+#    #+#             */
/*   Updated: 2025/05/18 17:16:32 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	word_count(char *str, char c)
{
	int		count;
	int		flag_pnt;
	size_t	i;

	count = 0;
	flag_pnt = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != c && flag_pnt == 0)
		{
			flag_pnt = 1;
			count++;
		}
		else if (str[i] == c)
			flag_pnt = 0;
		i++;
	}
	return (count);
}

static char	*fill_word(t_gcnode **gc, char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = gc_malloc(gc, (end - start + 1) * sizeof(char));
	while (start < end)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	return (word);
}

static char	**init_spl(t_gcnode **gc, char *s, char c, char **result)
{
	size_t	i;
	int		j;
	int		s_word;

	i = 0;
	j = 0;
	s_word = -1;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && s_word < 0)
			s_word = i;
		else if ((s[i] == c || i == ft_strlen(s)) && s_word >= 0)
		{
			result[j] = fill_word(gc, s, s_word, i);
			s_word = -1;
			j++;
		}
		i++;
	}
	return (result);
}

char	**gc_split(t_gcnode **gc, char *s, char c)
{
	char	**res;

	if (!s)
		return (NULL);
	res = gc_malloc(gc, (word_count(s, c) + 1) * sizeof(char *));
	return (init_spl(gc, s, c, res));
}
