/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:02:33 by mberila           #+#    #+#             */
/*   Updated: 2025/06/28 17:16:58 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(t_gcnode **gc, t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			gc_free(gc, cmd->args[i]);
			i++;
		}
		gc_free(gc, cmd->args);
	}
	if (cmd->redirections)
	{
		i = 0;
		while (i < cmd->redir_count)
			gc_free(gc, cmd->redirections[i++].file);
		gc_free(gc, cmd->redirections);
	}
	if (cmd->path)
		gc_free(gc, cmd->path);
	gc_free(gc, cmd);
}

void	free_commands(t_gcnode **gc, t_cmd *commands)
{
	t_cmd	*current;
	t_cmd	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		free_command(gc, current);
		current = next;
	}
}

void	free_tokens(t_gcnode **gc, t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		gc_free(gc, current->value);
		gc_free(gc, current);
		current = next;
	}
}
