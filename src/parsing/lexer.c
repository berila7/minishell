/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:07:53 by mberila           #+#    #+#             */
/*   Updated: 2025/05/20 19:51:03 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redout_tokens(t_data *data, t_token **token, char *line, int *i)
{
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			add_token(token, new_token(&data->gc, ">>", TOKEN_REDIR_APPEND));
			(*i) += 2;
		}
		else
		{
			add_token(token, new_token(&data->gc, ">", TOKEN_REDIR_OUT));
			(*i)++;
		}
		return (1);
	}
	return (0);
}

int	handle_redin_tokens(t_data *data, t_token **token, char *line, int *i)
{
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			add_token(token, new_token(&data->gc, "<<", TOKEN_HEREDOC));
			data->hered_count++;
			if (data->hered_count > 16)
			{
				print_err("minishell: maximum here-document count exceeded\n", NULL);
				gc_free_all(&data->gc);
				exit(2);
			}
			(*i) += 2;
		}
		else
		{
			add_token(token, new_token(&data->gc, "<", TOKEN_REDIR_IN));
			(*i)++;
		}
		return (1);
	}
	return (0);
}

int	handle_tokens(t_data *data, t_token **token, char *line, int *i)
{
	if (line[*i] == '|')
	{
		add_token(token, new_token(&data->gc, "|", TOKEN_PIPE));
		(*i)++;
		return (1);
	}
	else if (!handle_redin_tokens(data, token, line, i)
		&& !handle_redout_tokens(data, token, line, i))
		return (0);
	return (1);
}

t_token	*tokenize(char *line, t_data *data)
{
	t_token	*token;
	int		i;

	i = 0;
	token = NULL;
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (!handle_tokens(data, &token, line, &i))
			extract_word(&token, line, &i, data);
	}
	return (token);
}
