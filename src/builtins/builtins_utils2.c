/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:00:01 by anachat           #+#    #+#             */
/*   Updated: 2025/06/17 12:00:04 by anachat          ###   ########.fr       */
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
