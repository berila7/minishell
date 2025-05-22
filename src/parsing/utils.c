/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:28:40 by mberila           #+#    #+#             */
/*   Updated: 2025/05/22 17:29:32 by berila           ###   ########.fr       */
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

int	check_quotes_in_token(char *value)
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

void	process_quoted_token(t_gcnode **gc, char *expanded, t_cmd *current_cmd)
{
	char	*final_str;

	final_str = remove_quotes(gc, expanded);
	add_argument(gc, current_cmd, final_str);
	gc_free(gc, final_str);
}

void	process_unquoted_token(t_gcnode **gc, char *expanded,
	t_cmd *current_cmd)
{
	char	**split_words;
	int		i;

	split_words = gc_split(gc, expanded, ' ');
	i = 0;
	while (split_words[i])
	{
		add_argument(gc, current_cmd, split_words[i]);
		i++;
	}
	free_arr(gc, split_words);
}
