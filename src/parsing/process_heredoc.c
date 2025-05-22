/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:37:45 by berila            #+#    #+#             */
/*   Updated: 2025/05/22 15:37:50 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
