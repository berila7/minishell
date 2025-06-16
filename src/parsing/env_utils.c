/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:52:21 by berila            #+#    #+#             */
/*   Updated: 2025/06/16 10:39:59 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

int	count_nodes(t_env *env)
{
	t_env	*current;
	int		i;

	current = env;
	i = 0;
	while (current)
	{
		if (current->value)
			i++;
		current = current->next;
	}
	return (i);
}

char	**env_to_array(t_gcnode **gc, t_env *env)
{
	t_env	*current;
	char	**env_array;
	char	*temp;
	int		i;

	env_array = gc_malloc(gc, sizeof(char *) * (count_nodes(env) + 1));
	current = env;
	i = 0;
	while (current)
	{
		if (current->value)
		{
			temp = gc_strjoin(gc, current->key, "=");
			env_array[i] = gc_strjoin(gc, temp, current->value);
			gc_free(gc, temp);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	env_append(t_env **env, t_env *new_node)
{
	t_env	*current;

	if (!*env)
	{
		*env = new_node;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

void	set_default_env(t_data *data)
{
	char	*cwd;

	if (!env_exists(data->env, "OLDPWD"))
		set_env(&data->gc, &data->env, "OLDPWD", NULL);
	if (!env_exists(data->env, "PWD"))
	{
		cwd = getcwd(NULL, 0);
		set_env(&data->gc, &data->env, "PWD", cwd);
		free(cwd);
	}
}
