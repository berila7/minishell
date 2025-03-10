/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:17:20 by mberila           #+#    #+#             */
/*   Updated: 2024/11/15 11:12:36 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s, const char *sub, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	str = (char *)s;
	if (*sub == '\0')
		return (str);
	while (i < len && s[i])
	{
		if (s[i] == sub[0])
		{
			j = 0;
			while (i + j < len && sub[j] != '\0' && s[i + j] == sub[j])
				j++;
			if (sub[j] == '\0')
				return (str + i);
		}
		i++;
	}
	return (NULL);
}
