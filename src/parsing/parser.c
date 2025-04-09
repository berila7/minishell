/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/04/09 18:06:44 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*new_command()
{
	t_command	*cmd;

	cmd	= malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	
	
}

void	add_command(t_command **cmds, t_command *new_cmd)
{
	t_command	*current;
	if (!*cmds)
	{
		*cmds = new_cmd;
		return ;
	}
	current = *cmds;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

t_command	*parse_tokens(t_token *tokes)
{
	
}

void	free_commands(t_command *commands)
{
	t_command	*current;
	t_command	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		
		current = next;
	}
}