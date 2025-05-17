/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:57:20 by mberila           #+#    #+#             */
/*   Updated: 2025/05/17 10:22:08 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	size_t	len;
	char	*p_src;

	if (!src)
		return (NULL);
	len = ft_strlen(src) + 1;
	p_src = ft_calloc(sizeof(char), len);
	if (p_src == NULL)
		return (NULL);
	ft_strlcpy(p_src, src, len);
	return (p_src);
}
