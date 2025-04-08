/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/04/08 14:12:08 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int ac, char *av[])
{
	char	*line;
	t_token	*tokens;

	(void)ac;
	(void)av;
	while (1)
	{
		line = readline("minishell> ");
		if (!line || !ft_strncmp(line, "exit", 5))
			break ;
		if (line[0])
			add_history(line);
		tokens = tokenize(line);
		t_token	*current = tokens;
		while (current)
		{
			printf("Token: '%s', Type: %d\n", current->value, current->type);
			current = current->next;
		}
		free_tokens(tokens);
		free(line);
	}
	return (0);
}