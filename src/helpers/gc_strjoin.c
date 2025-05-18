/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:01:37 by anachat           #+#    #+#             */
/*   Updated: 2025/05/18 17:14:21 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*gc_strjoin(t_gcnode **gc, char *s1, char *s2)
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
	joined_str = gc_malloc(gc, (lenght + 1) * sizeof(char));
	while (s1[i])
		joined_str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		joined_str[j++] = s2[i++];
	return (joined_str);
}
