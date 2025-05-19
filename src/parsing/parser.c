/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:31:50 by mberila           #+#    #+#             */
/*   Updated: 2025/05/19 14:16:38 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_command(t_gcnode **gc)
{
	t_cmd	*cmd;

	cmd = gc_malloc(gc ,sizeof(t_cmd));
	cmd->args = gc_malloc(gc, sizeof(char *) * 2);
	cmd->hd_fd = -1;
	if (!cmd->args)
	{
		gc_free(gc, cmd);
		return (NULL);
	}
	cmd->args[0] = NULL;
	cmd->redirections = NULL;
	cmd->redir_count = 0;
	return (cmd);
}

void	add_argument(t_gcnode **gc, t_cmd *cmd, char *arg)
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
	new_args = gc_malloc(gc, sizeof(char *) * (size + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = gc_strdup(gc, arg);
	new_args[i + 1] = NULL;
	gc_free(gc, cmd->args);
	cmd->args = new_args;
}

void	add_redirection(t_gcnode **gc, t_cmd *cmd, int type, char *file)
{
	t_redir	*new_redirs;
	int		i;

	new_redirs = gc_malloc(gc, sizeof(t_redir) * (cmd->redir_count + 1));
	if (!new_redirs)
		return ;
	i = 0;
	while (i < cmd->redir_count)
	{
		new_redirs[i].type = cmd->redirections[i].type;
		new_redirs[i].file = cmd->redirections[i].file;
		i++;
	}
	new_redirs[cmd->redir_count].type = type;
	new_redirs[cmd->redir_count].file = gc_strdup(gc, file);
	gc_free(gc, cmd->redirections);
	cmd->redirections = new_redirs;
	cmd->redir_count++;
}

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

void	add_command(t_cmd **cmds, t_cmd *new_cmd, t_data *data)
{
	t_cmd	*current;

	data->is_export = 1;
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

t_cmd	*parse_tokens(t_gcnode **gc, t_token *tokens, t_data *data)
{
	t_token	*token;
	t_cmd	*current_cmd;
	t_cmd	*cmd_list;
	char	*expanded;

	// (void)expanded;
	if (!tokens)
		return (NULL);
	current_cmd = new_command(gc);
	cmd_list = NULL;
	if (!current_cmd)
		return (NULL);
	token = tokens;
	while (token)
	{
		if (token->type == TOKEN_WORD)
		{
			// expanded = expand_variables(token->value, data);
			// add_argument(current_cmd, expanded);
			// free(expanded);
			process_token_word(gc, token, current_cmd, data);
			token = token->next;
		}
		else if (token->type == TOKEN_PIPE)
		{
			add_command(&cmd_list, current_cmd, data);
			current_cmd = new_command(gc);
			if (!current_cmd)
			{
				free_commands(gc, cmd_list);
				return (NULL);
			}
			token = token->next;
		}
		else if (token->type == TOKEN_REDIR_IN)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				expanded = expand_variables(gc, token->value, data);
				add_redirection(gc, current_cmd, REDIR_IN, expanded);
				gc_free(gc, expanded);
				token = token->next;
			}
			else
			{
				printf("minishell: syntax error near unexpected token 'newline'\n");
				exit_status(2, 1);
				free_commands(gc, cmd_list);
				free_command(gc, current_cmd);
				return (NULL);
			}
		}
		else if (token->type == TOKEN_REDIR_OUT)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				expanded = expand_variables(gc, token->value, data);
				add_redirection(gc, current_cmd, REDIR_OUT, expanded);
				gc_free(gc, expanded);
				token = token->next;
			}
			else
			{
				printf("minishell: syntax error near unexpected token 'newline'\n");
				exit_status(2, 1);
				free_commands(gc, cmd_list);
				free_command(gc, current_cmd);
				return (NULL);
			}
		}
		else if (token->type == TOKEN_REDIR_APPEND)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				expanded = expand_variables(gc, token->value, data);
				add_redirection(gc, current_cmd, REDIR_APPEND, expanded);
				gc_free(gc, expanded);
				token = token->next;
			}
			else
			{
				printf("minishell: syntax error near unexpected token 'newline'\n");
				exit_status(2, 1);
				free_commands(gc, cmd_list);
				free_command(gc, current_cmd);
				return (NULL);
			}
		}
		else if (token->type == TOKEN_HEREDOC)
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
			{
				if (handle_herdoc(gc, token->value, &current_cmd->hd_fd, data))
				{
					free_commands(gc, cmd_list);
					free_command(gc, current_cmd);
					return (NULL);
				}
				add_redirection(gc, current_cmd, REDIR_HEREDOC, token->value);
				token = token->next;
			}
			else
			{
				printf("minishell: syntax error near unexpected token '<<'\n");
				exit_status(2, 1);
				free_commands(gc, cmd_list);
				free_command(gc, current_cmd);
				return (NULL);
			}
		}
	}
	add_command(&cmd_list, current_cmd, data);
	return (cmd_list);
}
