/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/04/10 11:37:51 by mberila          ###   ########.fr       */
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
	cmd->args = malloc(sizeof(char *) * 2);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args[0] = NULL;
	return cmd;
}

void	add_argument(t_command *cmd, char *arg)
{
	char	**new_args;
	int		i;
	int		size;

	i = 0;
	size = 0;
	while (cmd->args[i])
	{
		i++;
		size++;
	}
	new_args = malloc(sizeof(char *) * (size + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
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

t_command	*parse_tokens(t_token *tokens)
{
	t_token		*token;
	t_command	*current_cmd;
	t_command	*cmd_list;

	if (!tokens)
		return (NULL);
	current_cmd = new_command();
	cmd_list = NULL;
	if (!current_cmd)
		return (NULL);
	token = tokens;
	while (token)
	{
		/* code */
	}
	
}

void	free_command(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return (NULL);
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->heredoc_delim)
		free(cmd->heredoc_delim);
	free(cmd);
}

void	free_commands(t_command *commands)
{
	t_command	*current;
	t_command	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}