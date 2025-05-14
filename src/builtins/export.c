/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:50:01 by anachat           #+#    #+#             */
/*   Updated: 2025/05/14 11:52:53 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}

void	ft_export(char **args, t_data *data)
{
	char	*key;
	char	*val;
	char	*equals;
	int		i;

	if (count_args(args) == 1)
		print_env(data->env);
	else if (count_args(args) > 1)
	{
		i = 1;
		while (args[i])
		{
			equals = ft_strchr(args[i], '=');
			if (equals)
			{
				key = ft_substr(args[i], 0, (equals - args[i]));
				if (*(equals - 1) == '+')
				{
					key[ft_strlen(key) - 1] = '\0';
					val = get_env(data->env, key);
					if (!val)
						val = ft_strdup("");
					val = ft_strjoin(val, equals + 1);
				}
				else
					val = ft_strdup(equals + 1);
				set_env(&data->env, key, val);
			}
			else
				set_env(&data->env, key, NULL);	
			i++;
		}
	}
}
