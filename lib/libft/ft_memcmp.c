/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:10:13 by mberila           #+#    #+#             */
/*   Updated: 2024/11/15 11:11:13 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*byte_ptr1;
	unsigned char	*byte_ptr2;
	size_t			i;

	byte_ptr1 = (unsigned char *)s1;
	byte_ptr2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (byte_ptr1[i] != byte_ptr2[i])
		{
			return (byte_ptr1[i] - byte_ptr2[i]);
		}
		i++;
	}
	return (0);
}
