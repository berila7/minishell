/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:50:01 by anachat           #+#    #+#             */
/*   Updated: 2025/06/20 17:08:53 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_env(char *key, char *arg, t_data *data)
{
	char	*val;
	char	*equals;

	equals = ft_strchr(arg, '=');
	if (*(equals - 1) == '+')
	{
		val = get_env(data->env, key);
		if (!val)
			val = gc_strdup(&data->gc, "");
		val = gc_strjoin(&data->gc, val, equals + 1);
	}
	else
		val = gc_strdup(&data->gc, equals + 1);
	set_env(&data->gc, &data->env, key, val);
}

char	*get_key(t_gcnode **gc, char *arg)
{
	char	*equals;
	size_t	key_len;

	if (arg[0] == '=')
		return (NULL);
	equals = ft_strchr(arg, '=');
	if (equals)
	{
		key_len = equals - arg;
		if (arg[key_len - 1] == '+')
			key_len--;
		return (gc_substr(gc, arg, 0, key_len));
	}
	return (gc_strdup(gc, arg));
}

int	handle_env(char *arg, char *key, t_data *data)
{
	if (!is_valid_env_key(key))
	{
		print_err("export: '%s': not a valid identifier\n", arg);
		return (1);
	}
	else
	{
		if (ft_strchr(arg, '='))
			create_env(key, arg, data);
		else if (!env_exists(data->env, key))
			set_env(&data->gc, &data->env, key, NULL);
	}
	return (0);
}

int	ft_export(char **args, t_data *data)
{
	int		ext_st;
	char	*key;
	int		i;

	ext_st = 0;
	if (count_args(args) == 1)
		sorted_env(&data->gc, data->env);
	else if (count_args(args) > 1)
	{
		i = 1;
		while (args[i])
		{
			key = get_key(&data->gc, args[i]);
			if (handle_env(args[i], key, data))
				ext_st = 1;
			i++;
		}
	}
	return (ext_st);
}
