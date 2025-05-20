/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:43:30 by mberila           #+#    #+#             */
/*   Updated: 2025/05/19 14:53:42 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(t_gcnode **gc, char	**envp)
{
	t_env	*env_list;
	t_env	*new_node;
	char	*equals;
	int		key_len;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equals = ft_strchr(envp[i], '=');
		if (equals)
		{
			new_node = gc_malloc(gc, sizeof(t_env));
			key_len = equals - envp[i];
			new_node->key = gc_substr(gc, envp[i], 0, key_len);
			new_node->value = gc_strdup(gc, equals + 1);
			new_node->next = NULL;
			env_append(&env_list, new_node);
		}
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

void	set_env(t_gcnode **gc, t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_var;

	current = *env;
	while (current)
	{
		if (equal(current->key, key))
		{
			gc_free(gc, current->value);
			current->value = gc_strdup(gc, value);
			return ;
		}
		current = current->next;
	}
	new_var = gc_malloc(gc, sizeof(t_env));
	if (!new_var)
		return ;
	new_var->key = gc_strdup(gc, key);
	new_var->value = gc_strdup(gc, value);
	env_append(&(*env), new_var);
}

void	free_env(t_gcnode **gc, t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		gc_free(gc, current->key);
		gc_free(gc, current->value);
		gc_free(gc, current);
		current = next;
	}
}

void	unset_env(t_gcnode **gc, t_env **env, char *key)
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
			gc_free(gc, current->key);
			gc_free(gc, current->value);
			gc_free(gc, current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

// void	f(void)
// {
// 	system("leaks minishell");
// }

// int main(int ac, char *av[], char **envp)
// {
// char		**env_array;
// 	t_data *data;
// 	atexit(f);
// 	data = malloc(sizeof(t_data));
// 	if (!data)
// 		return (printf("error"), 1);
// 	data->env = init_env(envp);
// 	char *str = expand_variables("echo $USER", data->env, 1);
// 	printf("%s", str);
// env_array = env_to_array(data->env);
// int	i = 0;
// while (env_array[i])
// {
// 	printf("%s\n", env_array[i]);
// 	i++;
// }
// free_arr(env_array);
// free_env(data->env);
// free(data);
// return (0);
// }
