/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:50:01 by anachat           #+#    #+#             */
/*   Updated: 2025/05/19 12:26:00 by anachat          ###   ########.fr       */
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

	key = gc_substr(&data->gc, arg, 0, (equals - arg));
	if (*(equals - 1) == '+')
	{
		key[ft_strlen(key) - 1] = '\0';
		val = get_env(data->env, key);
		if (!val)
			val = gc_strdup(&data->gc, "");
		val = gc_strjoin(&data->gc, val, equals + 1);
	}
	else
		val = gc_strdup(&data->gc, equals + 1);
	set_env(&data->gc, &data->env, key, val);
}

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

void	ft_export(char **args, t_data *data)
{
	char	*equals;
	char	*key;
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
				create_env(equals, args[i], data);
			else if (!env_exists(data->env, args[i]))
			{
				printf("\n=====================> env key does not exists, create new envwith key: %s\n\n", key);
				key = gc_strdup(&data->gc, args[i]);
				set_env(&data->gc, &data->env, key, NULL);
			}
			i++;
		}
	}
}
