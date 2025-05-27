/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:21:36 by ayoub             #+#    #+#             */
/*   Updated: 2025/05/27 12:22:00 by ayoub            ###   ########.fr       */
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
