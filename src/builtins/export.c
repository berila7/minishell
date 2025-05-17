/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:50:01 by anachat           #+#    #+#             */
/*   Updated: 2025/05/17 18:42:29 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
	return (0);
}

static void	create_env(char *equals, char *arg, t_data *data)
{
	char	*key;
	char	*val;

	key = ft_substr(arg, 0, (equals - arg));
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

void	ft_export(char **args, t_data *data)
{
	char	*equals;
	char	*key;
	int		i;

	(void)key;
	if (count_args(args) == 1)
		print_env(data->env);
	else if (count_args(args) > 1)
	{
		i = 1;
		while (args[i])
		{
			equals = ft_strchr(args[i], '=');
			if (equals)
				create_env(equals, args[i], data);
			else
			{
				key = ft_strdup(args[i]);
				set_env(&data->env, key, NULL);
			}
			i++;
		}
	}
}