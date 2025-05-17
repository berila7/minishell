/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:43:30 by mberila           #+#    #+#             */
/*   Updated: 2025/05/17 10:23:20 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_to_env_list(t_env **env_list, t_env **last, t_env *new_node)
{
	if (!*env_list)
	{
		*env_list = new_node;
		*last = new_node;
	}
	else
	{
		(*last)->next = new_node;
		*last = new_node;
	}
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*last;
	t_env	*new_node;
	int		i;

	env_list = NULL;
	last = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (new_node)
			add_to_env_list(&env_list, &last, new_node);
		i++;
	}
	return (env_list);
}

char	*get_env(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	set_env(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_var;

	current = *env;
	while (current)
	{
		if (equal(current->key, key))
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->key = ft_strdup(key);
	if (!value)
		new_var->value = NULL;
	else
		new_var->value = ft_strdup(value);
	new_var->next = *env;
	*env = new_var;
}

void	unset_env(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
