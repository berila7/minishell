/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/04/10 15:20:55 by mberila          ###   ########.fr       */
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


void	free_command(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
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
		if (token->type == TOKEN_WORD)
		{
			add_argument(current_cmd, token->value);
			token = token->next;
		}
		else if (token->type == TOKEN_PIPE)
		{
			add_command(&cmd_list, current_cmd);
			current_cmd = new_command();
			if (!current_cmd)
			{
				free_commands(cmd_list);
				return (NULL);
			}
			token = token->next;
		}
		else if (token->type == TOKEN_REDIR_IN)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				current_cmd->input_file = ft_strdup(token->value);
				token = token->next;
			}
			else
			{
				free_commands(cmd_list);
				free_command(current_cmd);
				return (NULL);
			}
		}
		else if (token->type == TOKEN_REDIR_OUT)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				current_cmd->output_file = ft_strdup(token->value);
				current_cmd->apped_mode = 0;
				token = token->next;
			}
			else
			{
				free_commands(cmd_list);
				free_command(current_cmd);
				return (NULL);
			}
		}
		else if (token->type == TOKEN_REDIR_APPEND)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				current_cmd->output_file = ft_strdup(token->value);
				current_cmd->apped_mode = 1;
				token = token->next;
			}
			else
			{
				free_commands(cmd_list);
				free_command(current_cmd);
				return (NULL);
			}
		}
		else if (token->type == TOKEN_HEREDOC)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				current_cmd->heredoc_delim = ft_strdup(token->value);
				token = token->next;
			}
			else
			{
				free_commands(cmd_list);
				free_command(current_cmd);
				return (NULL);
			}
		}
	}
	add_command(&cmd_list, current_cmd);
	return (cmd_list);
}
