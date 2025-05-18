/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:28:40 by mberila           #+#    #+#             */
/*   Updated: 2025/05/18 18:59:01 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_status(int	status, int	is_accessor)
{
	static int	exit_status;
	if (is_accessor)
		exit_status = status;
	return (exit_status);
}

char	*ft_strjoin_char(t_gcnode **gc, char *str, char c)
{
	char	*result;
	int		i;

	result = gc_malloc(gc, ft_strlen(str) + 2);
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

char	*remove_quotes(t_gcnode **gc, char *str)
{
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;
	char	*result;

	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	result = gc_malloc(gc, noquotes_len(str) + 1);
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

char	*ft_strjoin_free(t_gcnode **gc, char *s1, char *s2)
{
	char	*result;

	result = gc_strjoin(gc, s1, s2);
	free(s1);
	return (result);
}

char	*ft_strjoin_char_free(t_gcnode **gc, char *str, char c)
{
	char	*result;

	result = ft_strjoin_char(gc, str, c);
	free(str);
	return (result);
}

char	*word_split_join(t_gcnode **gc, char *dest, char *src)
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
			if (!was_space)
			{
				result = ft_strjoin_char_free(gc, result, ' ');
				was_space = 1;
			}
			while (src[i] && (src[i] == ' ' || src[i] == '\t'))
				i++;
		}
		else
		{
			result = ft_strjoin_char_free(gc, result, src[i]);
			was_space = 0;
			i++;
		}
	}
	return (result);
}

void process_token_word(t_gcnode **gc, t_token *token, t_cmd *current_cmd, t_data *data)
{
    char *expanded;
    char **split_words;
    int i;
    char *final_str;
    
    expanded = expand_variables(gc, token->value, data);
    
    int has_quotes = 0;
    i = 0;
    while (token->value[i])
    {
        if (token->value[i] == '\'' || token->value[i] == '\"')
            has_quotes = 1;
        i++;
    }
    
    if (has_quotes || !data->is_export)
    {
        // For quoted strings, we need to remove quotes but preserve quoted content
        final_str = remove_quotes(gc, expanded);
        add_argument(gc, current_cmd, final_str);
        free(final_str);
    }
    else
    {
        split_words = gc_split(gc, expanded, ' ');
        i = 0;
        while (split_words[i])
        {
            add_argument(gc, current_cmd, split_words[i]);
            i++;
        }
        free_arr(split_words);
    }
    
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