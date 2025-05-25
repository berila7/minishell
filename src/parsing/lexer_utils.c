/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:17:51 by berila            #+#    #+#             */
/*   Updated: 2025/05/24 18:41:54 by berila           ###   ########.fr       */
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
	int		in_quote;

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
	free_tokens(&data->gc, *tokens);
	*tokens = NULL;
	return (1);
}

t_token	*prev_token(t_token *token, t_token *current_token)
{
	t_token	*prev;
	t_token	*temp;

	if (!token || !current_token || token == current_token)
		return (NULL);
	
	prev = NULL;
	temp = token;
	
	while (temp && temp != current_token)
	{
		prev = temp;
		temp = temp->next;
	}
	
	if (temp == current_token)
		return (prev);
	return (NULL);
}

void	extract_word(t_token **tokens, char *line, int *i, t_data *data)
{
	int		start;
	int		in_quote;
	char	*word;

	start = *i;
	in_quote = toggel_quote(line, i);
	if (handle_quote_error(tokens, data, in_quote))
		return ;
	if (*i > start)
	{
		word = gc_substr(&data->gc, line, start, *i - start);
		add_token(tokens, new_token(&data->gc, word, TOKEN_WORD));
		if ((equal((*tokens)->value, "export"))
			||( (*tokens)->type == TOKEN_REDIR_APPEND
			|| (*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT)
		)
			data->is_export = 0;
		if (((*tokens)->next && equal((*tokens)->value, "export"))
			&& (*tokens)->next->value[0] == '$'
		)
			data->is_export = 1;
		if ((*tokens)->next && (*tokens)->next->next && !equal((*tokens)->next->next->value, "export"))
			data->is_export = 1;
		gc_free(&data->gc, word);
	}
}
