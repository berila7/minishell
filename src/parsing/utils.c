/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:28:40 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 19:54:53 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	char	*result;
	int		i;

	result = malloc(ft_strlen(str) + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
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

int	noquotes_len(char *str)
{
	int	len;
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	len = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			len++;
		i++;
	}
	return (len);
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;
	char	*result;

	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	result = malloc(noquotes_len(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	free_data(t_data *data)
{
	if (data)
	{
		if (data->cmds)
			free_commands(data->cmds);
		if (data->env)
			free_env(data->env);
		free(data);
	}
}

int	validate_token(t_token *token)
{
	t_token	*current;
	int		prev_pip;

	current = token;
	prev_pip = 1;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (prev_pip)
				return (printf("minishell: \
					syntax error near unexpected token '|'\n"), 0);
			prev_pip = 1;
		}
		else
			prev_pip = 0;
		current = current->next;
	}
	if (prev_pip)
		return (printf("minishell: syntax error: \
			unexpected end of file\n"), 0);
	return (1);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_strjoin_char_free(char *str, char c)
{
	char	*result;

	result = ft_strjoin_char(str, c);
	free(str);
	return (result);
}

static int	skip_spaces(char *src, int i)
{
	while (src[i] && (src[i] == ' ' || src[i] == '\t'))
		i++;
	return (i);
}

static char	*handle_space(char *result, int *was_space)
{
	if (!*was_space)
	{
		result = ft_strjoin_char_free(result, ' ');
		*was_space = 1;
	}
	return (result);
}

char	*word_split_join(char *dest, char *src)
{
	int		i;
	int		was_space;
	char	*result;

	i = 0;
	was_space = 1;
	result = dest;
	while (src[i])
	{
		if (src[i] == ' ' || src[i] == '\t')
		{
			result = handle_space(result, &was_space);
			i = skip_spaces(src, i);
		}
		else
		{
			result = ft_strjoin_char_free(result, src[i]);
			was_space = 0;
			i++;
		}
	}
	return (result);
}

static int	has_quotes_in_token(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

static void	handle_quoted_word(char *expanded, t_cmd *current_cmd)
{
	char	*final_str;

	final_str = remove_quotes(expanded);
	add_argument(current_cmd, final_str);
	free(final_str);
}

static void	handle_unquoted_word(char *expanded, t_cmd *current_cmd)
{
	char	**split_words;
	int		i;

	if (ft_strchr(expanded, '='))
	{
		add_argument(current_cmd, expanded);
		return ;
	}
	split_words = ft_split(expanded, ' ');
	i = 0;
	while (split_words[i])
	{
		add_argument(current_cmd, split_words[i]);
		i++;
	}
	free_arr(split_words);
}

void	process_token_word(t_token *token, t_cmd *current_cmd, t_data *data)
{
	char	*expanded;

	expanded = expand_variables(token->value, data);
	if (has_quotes_in_token(token->value))
		handle_quoted_word(expanded, current_cmd);
	else
		handle_unquoted_word(expanded, current_cmd);
	free(expanded);
}

// char	*remove_escape_chars(char *str)
// {
// 	int		i;
// 	int		j;
// 	int		escaped;
// 	int		in_single_quote;
// 	int		in_double_quote;
// 	char	*result;
// 	int		len;

// 	i = 0;
// 	j = 0;
// 	in_single_quote = 0;
// 	in_double_quote = 0;
// 	escaped = 0;
// 	len = ft_strlen(str);
// 	result = malloc(len + 1);
// 	if (!result)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		if (str[i] == '\'' && !escaped && !in_double_quote)
// 		{
// 			in_single_quote = !in_single_quote;
// 			result[j++] = str[i++];
// 		}
// 		else if (str[i] == '\"' && !escaped && !in_single_quote)
// 		{
// 			in_double_quote = !in_double_quote;
// 			result[j++] = str[i++];
// 		}
// 		else if (str[i] == '\\' && !escaped && !in_single_quote)
// 		{
// 			escaped = 1;
// 			if (str[i + 1] == '\'' || str[i + 1] == '\"')
// 			{
// 				result[j++] = 1;
// 				result[j++] = str[i + 1];
// 				i += 2;
// 				escaped = 0;
// 			}
// 			else
// 				i++;
// 		}
// 		else
// 		{
// 			result[j++] = str[i++];
// 			escaped = 0;
// 		}
// 	}
// 	result[j] = '\0';
// 	return (result);
// }