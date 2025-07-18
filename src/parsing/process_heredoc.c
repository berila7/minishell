/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:01:39 by mberila           #+#    #+#             */
/*   Updated: 2025/06/28 15:18:22 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*restore_quotes(t_gcnode **gc, char *str)
{
	int		i;
	int		len;
	char	*result;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = gc_malloc(gc, len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (str[i] == SINGLE_QUOTE)
			result[i] = '\'';
		else if (str[i] == DOUBLE_QUOTE)
			result[i] = '"';
		else
			result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*remove_quotes(t_gcnode **gc, char *str)
{
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;
	char	*result;

	if (!str)
		return (NULL);
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	result = gc_malloc(gc, noquotes_len(str) + 1);
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
	return (restore_quotes(gc, result));
}

int	process_heredoc_token(t_token **token, t_cmd *current_cmd, t_cmd *cmd_list,
				t_data *data)
{
	*token = (*token)->next;
	if (*token && (*token)->type == TOKEN_WORD)
	{
		if (handle_herdoc((*token),
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
		print_err("syntax error near %s", "unexpected token 'newline'\n");
		exit_status(2, 1);
		free_commands(&data->gc, cmd_list);
		free_command(&data->gc, current_cmd);
		return (0);
	}
}

char	*ptr_to_hex_str(t_gcnode **gc, void *ptr)
{
	unsigned long	addr;
	char			*str;
	char			*hex;
	int				i;

	addr = (unsigned long)ptr;
	str = gc_malloc(gc, 17);
	if (!str)
		return (NULL);
	str[16] = '\0';
	hex = "0123456789abcdef";
	i = 15;
	while (addr)
	{
		str[i--] = hex[addr % 16];
		addr /= 16;
	}
	while (i >= 0)
		str[i--] = '0';
	return (str);
}
