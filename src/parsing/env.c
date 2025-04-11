/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:43:30 by mberila           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/11 16:41:44 by mberila          ###   ########.fr       */
=======
/*   Updated: 2025/04/11 16:33:51 by anachat          ###   ########.fr       */
>>>>>>> 59e8fd438bd48c1a2609a19c6bd2b2ea8ad067cf
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(char	**envp)
{
	t_env	*env_list;
	t_env	*new_node;
	char 	*equals;
	int		key_len;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equals = ft_strchr(envp[i], '=');
		if (equals)
		{
			new_node = malloc(sizeof(t_env));
			if (!new_node)
				return (NULL);
			key_len = equals - envp[i];
			new_node->key = ft_substr(envp[i], 0, key_len);
			new_node->value = ft_strdup(equals + 1);
			new_node->next = env_list;
			env_list = new_node;
		}
		i++;
	}
	return (env_list);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if(ft_strncmp(current->key, key, ft_strlen(key)) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

// char	*expand_env_vars(t_env *env, char *str, int exit_status)
// {
	
// }
