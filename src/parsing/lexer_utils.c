/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:17:51 by berila            #+#    #+#             */
/*   Updated: 2025/06/16 15:09:51 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(t_gcnode **gc, char *value, t_token_type type)
{
	t_token	*token;

	token = gc_malloc(gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = gc_strdup(gc, value);
	if (!token->value)
	{
		gc_free(gc, token);
		return (NULL);
	}
	token->type = type;
	check_quotes(&token);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		new_token->prev = NULL;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
	new_token->next = NULL;
}

int	toggel_quote(char *line, int *i)
{
	int	in_quote;

	in_quote = 0;
	while (line[*i])
	{
		if (line[*i] == '\'')
		{
			if (in_quote == 0)
				in_quote = 1;
			else if (in_quote == 1)
				in_quote = 0;
		}
		else if (line[*i] == '\"')
		{
			if (in_quote == 0)
				in_quote = 2;
			else if (in_quote == 2)
				in_quote = 0;
		}
		if (!in_quote && (line[*i] == ' ' || line[*i] == '\t'
				|| line[*i] == '|' || line[*i] == '<' || line[*i] == '>'))
			break ;
		(*i)++;
	}
	return (in_quote);
}

int	handle_quote_error(t_token **tokens, t_data *data, int in_quote)
{
	char	*quote_state;

	if (in_quote == 0)
		return (0);
	if (in_quote == 1)
		quote_state = "single";
	else
		quote_state = "double";
	printf("minishell: syntax error: unclosed %s quote\n",
		quote_state);
	exit_status(2, 1);
	free_tokens(&data->gc, *tokens);
	*tokens = NULL;
	return (1);
}
