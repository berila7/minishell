/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:43:30 by mberila           #+#    #+#             */
/*   Updated: 2025/04/11 15:30:08 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

// char	*get_env_value(t_env *env, char *key)
// {

// }

// char	*expand_env_vars(t_env *env, char *str, int exit_status)
// {
	
// }
