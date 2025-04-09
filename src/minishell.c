/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/04/09 10:56:31 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_header()
{
	printf(WHITE"\n███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     ");
	printf("\n████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     ");
	printf("\n██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     ");
	printf("\n██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     ");
	printf("\n██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗");
	printf("\n╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n\n" RESET);
}

int main(int ac, char *av[])
{
	char	*line;
	t_token	*tokens;

	print_header();
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