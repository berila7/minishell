/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:21:36 by ayoub             #+#    #+#             */
/*   Updated: 2025/06/13 16:08:06 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_exists(t_env *env, char *key)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (equal(key, curr->key))
			return (1);
		curr = curr->next;
	}
	return (0);
}
