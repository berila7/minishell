/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:57:20 by mberila           #+#    #+#             */
/*   Updated: 2024/11/10 10:48:24 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	size_t	len;
	char	*p_src;

	len = ft_strlen(src) + 1;
	p_src = ft_calloc(sizeof(char), len);
	if (p_src == NULL)
		return (NULL);
	ft_strlcpy(p_src, src, len);
	return (p_src);
}
