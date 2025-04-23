/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:28:40 by mberila           #+#    #+#             */
/*   Updated: 2025/04/23 09:49:25 by anachat          ###   ########.fr       */
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

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	int		len;
	int 	in_single_quote;
	int		in_double_quote;
	char	*result;

	i = 0;
	j = 0;
	len = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if(str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			len++;
		i++;
	}
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if(str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
		{
			result[j] = str[i];
			j++;
		}
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
				return (printf(RED"minishell: syntax error near unexpected token '|'\n"RESET), 0);
			prev_pip = 1;
		}
		else
			prev_pip = 0;
		current = current->next;
	}
	if (prev_pip)
		return (printf(RED"minishell: syntax error: unexpected end of file\n"RESET), 0);
	return (1);
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