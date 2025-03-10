/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:01:26 by mberila           #+#    #+#             */
/*   Updated: 2024/11/10 10:48:06 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_count(const char *str, char c)
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

static char	*fill_word(const char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = ft_calloc((end - start + 1), sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	return (word);
}

static void	*free_all(char **strs, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (NULL);
}

static char	**init_spl(const char *s, char c, char **result)
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
			result[j] = fill_word(s, s_word, i);
			if (!(result[j]))
				return (free_all(result, j));
			s_word = -1;
			j++;
		}
		i++;
	}
	return (result);
}

char	**ft_split(const char *s, char c)
{
	char	**res;

	if (!s)
		return (NULL);
	res = ft_calloc((word_count(s, c) + 1), sizeof(char *));
	if (!res)
		return (NULL);
	return (init_spl(s, c, res));
}
