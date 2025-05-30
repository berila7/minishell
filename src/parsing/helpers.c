/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:25:34 by berila            #+#    #+#             */
/*   Updated: 2025/05/30 11:45:22 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_handler(t_token **tokens, t_data *data)
{
	char	*after_equ;

	after_equ = NULL;
	if ((equal((*tokens)->value, "export"))
		|| ((*tokens)->type == TOKEN_REDIR_APPEND
			|| (*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT)
	)
		data->is_export = 0;
	if ((*tokens)->next)
		after_equ = ft_strchr((*tokens)->next->value, '=');
	if (((*tokens)->next && equal((*tokens)->value, "export"))
		&& (*tokens)->next->value[0] == '$'
		&& ft_strchr(after_equ, '$')
	)
		data->is_export = 1;
	if ((*tokens)->next && (*tokens)->next->next
		&& !equal((*tokens)->next->next->value, "export"))
	{
		data->is_export = 1;
	}
}
