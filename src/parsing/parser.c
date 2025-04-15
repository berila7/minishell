/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/04/15 09:53:27 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_command()
{
	t_cmd	*cmd;

	cmd	= malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->args = malloc(sizeof(char *) * 2);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args[0] = NULL;
	return cmd;
}

void	add_argument(t_cmd *cmd, char *arg)
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


void	free_command(t_cmd *cmd)
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
	if (cmd->path)
		free(cmd->path);
	free(cmd);
}

void	free_commands(t_cmd *commands)
{
	t_cmd	*current;
	t_cmd	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}

void	add_command(t_cmd **cmds, t_cmd *new_cmd)
{
	t_cmd	*current;
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

t_cmd	*parse_tokens(t_token *tokens)
{
	t_token		*token;
	t_cmd	*current_cmd;
	t_cmd	*cmd_list;

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
				printf(RED"minishell: syntax error near unexpected token '\\n'\n"RESET);
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
				current_cmd->append_mode = 0;
				token = token->next;
			}
			else
			{
				printf(RED"minishell: syntax error near unexpected token '\\n'\n"RESET);
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
				current_cmd->append_mode = 1;
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
				printf(RED"minishell: syntax error near unexpected token '<<'\n"RESET);
				free_commands(cmd_list);
				free_command(current_cmd);
				return (NULL);
			}
		}
	}
	add_command(&cmd_list, current_cmd);
	return (cmd_list);
}
