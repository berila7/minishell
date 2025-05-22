/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:37:45 by berila            #+#    #+#             */
/*   Updated: 2025/05/22 17:06:19 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	process_heredoc_token(t_token **token, t_cmd *current_cmd, t_cmd *cmd_list,
				t_data *data)
{
	*token = (*token)->next;
	if (*token && (*token)->type == TOKEN_WORD)
	{
		if (handle_herdoc(&data->gc, (*token)->value,
				&current_cmd->hd_fd, data))
		{
			free_commands(&data->gc, cmd_list);
			free_command(&data->gc, current_cmd);
			return (0);
		}
		add_redirection(&data->gc, current_cmd, REDIR_HEREDOC, (*token)->value);
		*token = (*token)->next;
		return (1);
	}
	else
	{
		printf("minishell: syntax error near unexpected token '<<'\n");
		exit_status(2, 1);
		free_commands(&data->gc, cmd_list);
		free_command(&data->gc, current_cmd);
		return (0);
	}
}
