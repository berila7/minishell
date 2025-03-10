/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:55:44 by mberila           #+#    #+#             */
/*   Updated: 2024/11/15 11:11:35 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int c, size_t n)
{
	unsigned char	*byte_ptr;
	unsigned char	target;
	size_t			i;

	byte_ptr = (unsigned char *)ptr;
	target = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (byte_ptr[i] == target)
			return ((void *)(byte_ptr + i));
		i++;
	}
	return (NULL);
}
