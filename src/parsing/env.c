/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:43:30 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 17:09:39 by berila           ###   ########.fr       */
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

static t_env	*create_env_node(char *env_var)
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

static void	init_expand_vars(t_expand *exp)
{
	exp->in_single_quote = 0;
	exp->in_double_quote = 0;
	exp->i = 0;
}

static void	handle_quotes(char *str, t_expand *exp)
{
	if (str[exp->i] == '\'' && !exp->in_double_quote)
	{
		exp->in_single_quote = !exp->in_single_quote;
		exp->result = ft_strjoin_char_free(exp->result, str[exp->i]);
		exp->i++;
	}
	else if (str[exp->i] == '\"' && !exp->in_single_quote)
	{
		exp->in_double_quote = !exp->in_double_quote;
		exp->result = ft_strjoin_char_free(exp->result, str[exp->i]);
		exp->i++;
	}
}

static void	handle_special_var(char *str, t_expand *exp, t_data *data)
{
	char	*status_str;
	char	first_digit;

	if (str[exp->i] == '?')
	{
		status_str = ft_itoa(data->exit_status);
		exp->result = ft_strjoin_free(exp->result, status_str);
		free(status_str);
		exp->i++;
	}
	else if (ft_isdigit(str[exp->i]))
	{
		first_digit = str[exp->i++];
		if (first_digit == '0')
			exp->result = ft_strjoin_free(exp->result, "minishell");
	}
}

static void	expand_env_var(char *str, t_expand *exp, t_data *data)
{
	int		start;
	char	*var_name;
	char	*var_value;

	start = exp->i;
	while (str[exp->i] && (is_valid_var_char(str[exp->i])))
		exp->i++;
	var_name = ft_substr(str, start, exp->i - start);
	var_value = get_env(data->env, var_name);
	free(var_name);
	if (var_value)
	{
		if (exp->in_double_quote)
			exp->result = ft_strjoin_free(exp->result, var_value);
		else
			exp->result = word_split_join(exp->result, var_value);
	}
}

static void	process_dollar(char *str, t_expand *exp, t_data *data)
{
	exp->i++;
	if (str[exp->i] == '?' || ft_isdigit(str[exp->i]))
		handle_special_var(str, exp, data);
	else if (str[exp->i] && is_valid_var_char(str[exp->i]))
		expand_env_var(str, exp, data);
	else
		exp->result = ft_strjoin_char_free(exp->result, '$');
}

char	*expand_variables(char *str, t_data *data)
{
	t_expand	exp;

	exp.result = ft_strdup("");
	if (!exp.result)
		return (NULL);
	init_expand_vars(&exp);
	while (str[exp.i])
	{
		if (str[exp.i] == '\'' || str[exp.i] == '\"')
			handle_quotes(str, &exp);
		else if (str[exp.i] == '$' && str[exp.i + 1]
			&& (!exp.in_single_quote || data->in_heredoc))
			process_dollar(str, &exp, data);
		else
		{
			exp.result = ft_strjoin_char_free(exp.result, str[exp.i]);
			exp.i++;
		}
	}
	return (exp.result);
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
