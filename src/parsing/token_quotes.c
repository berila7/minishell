/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:10:50 by mberila           #+#    #+#             */
/*   Updated: 2025/06/11 14:19:47 by mberila          ###   ########.fr       */
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
