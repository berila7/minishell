/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:01:45 by mberila           #+#    #+#             */
/*   Updated: 2025/05/16 20:18:02 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_expand_vars(t_expand *exp)
{
	exp->in_single_quote = 0;
	exp->in_double_quote = 0;
	exp->i = 0;
}

void	init_parser(t_cmd **current_cmd, t_cmd **cmd_list)
{
	*current_cmd = new_command();
	*cmd_list = NULL;
}

int	has_quotes_in_token(char *str)
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

void	handle_quoted_word(char *expanded, t_cmd *current_cmd)
{
	char	*final_str;

	final_str = remove_quotes(expanded);
	add_argument(current_cmd, final_str);
	free(final_str);
}

void	handle_unquoted_word(char *expanded, t_cmd *current_cmd)
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
