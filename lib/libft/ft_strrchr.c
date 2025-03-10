/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:31:45 by mberila           #+#    #+#             */
/*   Updated: 2024/11/09 17:32:44 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	const char	*last_oc;
	char		last;

	last_oc = NULL;
	last = (char) c;
	while (*str != '\0')
	{
		if (*str == last)
			last_oc = str;
		str++;
	}
	if (last == '\0')
		return ((char *)str);
	return ((char *)last_oc);
}
