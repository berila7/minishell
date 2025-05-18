/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:05:28 by anachat           #+#    #+#             */
/*   Updated: 2025/05/18 17:16:13 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*gc_strdup(t_gcnode **gc, char *src)
{
	size_t	len;
	char	*p_src;

	if (!src)
		return (NULL);
	len = ft_strlen(src) + 1;
	p_src = gc_malloc(gc, sizeof(char) * len);
	ft_strlcpy(p_src, src, len);
	return (p_src);
}
