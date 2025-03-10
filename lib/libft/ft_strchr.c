/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:50:12 by mberila           #+#    #+#             */
/*   Updated: 2024/11/09 17:07:29 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	first;
	char	*str;
	size_t	i;

	first = (char)c;
	str = (char *)s;
	i = 0;
	while (str[i])
	{
		if (str[i] == first)
			return (&str[i]);
		i++;
	}
	if (first == '\0' && str[i] == '\0')
		return (&str[i]);
	return (NULL);
}
