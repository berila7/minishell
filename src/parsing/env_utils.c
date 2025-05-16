/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:32:28 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 18:37:05 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int	count_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	current = env;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	fill_env_array(t_env *env, char **env_array)
{
	t_env	*current;
	int		i;
	char	*temp;

	current = env;
	i = 0;
	while (current)
	{
		temp = ft_strjoin(current->key, "=");
		env_array[i] = ft_strjoin(temp, current->value);
		free(temp);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
}

char	**env_to_array(t_env *env)
{
	char	**env_array;
	int		count;

	count = count_env_vars(env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	fill_env_array(env, env_array);
	return (env_array);
}

t_env	*create_env_node(char *env_var)
{
	t_env	*new_node;
	char	*equals;
	int		key_len;

	equals = ft_strchr(env_var, '=');
	if (!equals)
		return (NULL);
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	key_len = equals - env_var;
	new_node->key = ft_substr(env_var, 0, key_len);
	new_node->value = ft_strdup(equals + 1);
	new_node->next = NULL;
	return (new_node);
}
