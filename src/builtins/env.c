/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:01:21 by mberila           #+#    #+#             */
/*   Updated: 2025/05/23 16:45:27 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_cmd *cmd, t_env *env)
{
	t_env	*current;

	if (count_args(cmd->args) > 1)
	{
		print_err("env: '%s': No such file or directory\n", cmd->args[1]);
		return (127);
	}
	current = env;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
