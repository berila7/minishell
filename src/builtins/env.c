/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:01:21 by mberila           #+#    #+#             */
/*   Updated: 2025/04/11 16:33:29 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}