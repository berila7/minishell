/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:10:50 by mberila           #+#    #+#             */
/*   Updated: 2025/06/25 11:20:07 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	double_quotes(int *type, int *quote_type)
{
	if (*type == 0)
		*type = 2;
	else if (*type == 2)
	{
		*type = 0;
		*quote_type = 2;
	}
}

static void	single_quotes(int *type, int *quote_type)
{
	if (*type == 0)
		*type = 1;
	else if (*type == 1)
	{
		*type = 0;
		*quote_type = 1;
	}
}

void	check_quotes(t_token **token)
{
	int	i;
	int	quotes_type;

	i = 0;
	quotes_type = 0;
	(*token)->quote_type = 0;
	while ((*token)->value[i])
	{
		if ((*token)->value[i] == '\'')
			single_quotes(&quotes_type, &(*token)->quote_type);
		else if ((*token)->value[i] == '\"')
			double_quotes(&quotes_type, &(*token)->quote_type);
		i++;
	}
}

char	*get_arg(t_gcnode **gc, char *file)
{
	char	**array;
	char	*result;
	int		i;
	char	**args;

	args = gc_split(gc, file);
	array = gc_malloc(gc, sizeof(char *) * (gc_word_count_quoted(file) + 1));
	i = 0;
	while (args[i])
	{
		array[i] = gc_strdup(gc, args[i]);
		i++;
	}
	array[i] = NULL;
	result = gc_strdup(gc, array[0]);
	free_arr(gc, args);
	free_arr(gc, array);
	return (remove_quotes(gc, result));
}
