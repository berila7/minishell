/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:45:06 by mberila           #+#    #+#             */
/*   Updated: 2024/11/15 10:49:14 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined_str;
	int		i;
	int		j;
	size_t	lenght;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		lenght = ft_strlen(s2);
	else if (!s2)
		lenght = ft_strlen(s1);
	else
		lenght = ft_strlen(s1) + ft_strlen(s2);
	joined_str = ft_calloc((lenght + 1), sizeof(char));
	if (!joined_str)
		return (NULL);
	while (s1[i])
		joined_str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		joined_str[j++] = s2[i++];
	return (joined_str);
}
