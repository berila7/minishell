/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:43:30 by mberila           #+#    #+#             */
/*   Updated: 2025/04/17 10:47:42 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	**env_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	int		i;
	char	*temp;

	current = env;
	i = 0;
	while (current)
	{
		i++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (i + 1));
	if (!env_array)
		return (NULL);
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
	return (env_array);
}

t_env	*init_env(char	**envp)
{
	t_env	*env_list;
	t_env	*last;
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
			new_node->next = NULL;
			if (!env_list)
			{
				env_list = new_node;
				last = new_node;
			}
			else
			{
				last->next = new_node;
				last = new_node;
			}
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
		if(ft_strcmp(current->key, key) == 0)
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
		new_var->value = ft_strdup("");
	else
		new_var->value = ft_strdup(value);
	new_var->next = *env;
	*env = new_var;
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
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

char	*expand_variables(char *str, t_env *env, int exit_status)
{
	int		i;
	char	*result;
	char	*status_str;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		in_single_quote;
	int		in_double_quote;
	int		escaped;
	int		start;
	char	*final_result;
	char	*unescaped_result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	in_single_quote = 0;
	in_double_quote = 0;
	escaped = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && !escaped && !(in_single_quote))
		{
			escaped = 1;
			temp = ft_strjoin_char(result, str[i]);
			free(result);
			result = temp;
			i++;
			continue ;
		} 
		if (str[i] == '\'' && !escaped && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			temp = ft_strjoin_char(result, str[i]);
			free(result);
			result = temp;
			i++;
		}
		else if (str[i] == '\"' && !escaped && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			temp = ft_strjoin_char(result, str[i]);
			free(result);
			result = temp;
			i++;
		}
		else if (str[i] == '$' && str[i + 1] && !in_single_quote && !escaped)
		{
			i++;
			if (str[i] == '?')
			{
				status_str = ft_itoa(exit_status);
				temp = ft_strjoin(result, status_str);
				free(result);
				free(status_str);
				result = temp;
				i++;
			}
			else if (ft_isdigit(str[i]))
			{
				char digit = str[i];
				i++;
				if (digit == '0')
				{
					temp = ft_strjoin(result, "minishell");
					free(result);
					result = temp;
				}
			}
			else
			{
				start = i;
				while (str[i] && is_valid_var_char(str[i]))
					i++;
				if (i > start)
				{
					var_name = ft_substr(str, start, i - start);
					var_value = get_env(env, var_name);
					if (var_value)
					{
						temp = ft_strjoin(result, var_value);
						free(result);
						result = temp;
					}
					free(var_name);
				}
				else
				{
					temp = ft_strjoin_char(result, '$');
					free(result);
					result = temp;
				}
			}
		}
		else
		{
			temp = ft_strjoin_char(result, str[i]);
			free(result);
			result = temp;
			i++;
			escaped = 0;
		}
	}
	unescaped_result = remove_escape_chars(result);
	free(result);
    final_result = remove_quotes(unescaped_result);
    free(unescaped_result);
    return (final_result);
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
