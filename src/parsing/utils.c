/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:28:40 by mberila           #+#    #+#             */
/*   Updated: 2025/06/03 19:26:20 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			{
				return (exit_status(2, 1),
					print_err("syntax error near token '|'\n", NULL), 0);
			}
			prev_pip = 1;
		}
		else
			prev_pip = 0;
		current = current->next;
	}
	if (prev_pip)
		return (exit_status(2, 1),
			print_err("syntax error:unexpected %s",
				"end of file\n"), 0);
	return (1);
}

int	in_quotes(char *value)
{
	int	i;
	int	has_quotes;

	has_quotes = 0;
	i = 0;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '\"')
			has_quotes = 1;
		i++;
	}
	return (has_quotes);
}

int	in_quotes_two(char *value)
{
	int	i;
	int	has_quotes;

	has_quotes = 0;
	i = 0;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '\"')
			has_quotes = 1;
		i++;
	}
	return (has_quotes);
}

void	process_unquoted_token(t_token *token, t_data *data, char *expanded,
	t_cmd *current_cmd)
{
	char	**split_words;
	int		i;

	split_words = gc_split(&data->gc, expanded);
	i = 0;
	while (split_words[i])
	{
		add_argument(token, data, current_cmd, split_words[i]);
		i++;
	}
	free_arr(&data->gc, split_words);
}
