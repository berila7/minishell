/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:09:10 by mberila           #+#    #+#             */
/*   Updated: 2024/11/15 15:09:28 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;
	size_t	len;

	src_len = ft_strlen(src);
	if (!dst && size == 0 && src != NULL)
		return (src_len);
	dst_len = ft_strlen(dst);
	if (dst_len < size)
		len = (dst_len + src_len);
	else
		len = (src_len + size);
	if (size == 0)
		return (len);
	i = 0;
	while (src[i] != '\0' && (dst_len + i) < (size - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (len);
}
