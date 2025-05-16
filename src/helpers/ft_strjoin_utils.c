/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:07:42 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 20:09:57 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	char	*result;
	int		i;

	result = malloc(ft_strlen(str) + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*word_split_join(char *dest, char *src)
{
	int		i;
	int		was_space;
	char	*result;

	i = 0;
	was_space = 1;
	result = dest;
	while (src[i])
	{
		if (src[i] == ' ' || src[i] == '\t')
		{
			result = handle_space(result, &was_space);
			i = skip_spaces(src, i);
		}
		else
		{
			result = ft_strjoin_char_free(result, src[i]);
			was_space = 0;
			i++;
		}
	}
	return (result);
}
