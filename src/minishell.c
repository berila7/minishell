/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/04/08 10:41:51 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int ac, char *av[])
{
	char	*line;

	(void)ac;
	(void)av;
	while (1)
	{
		line = readline("minishell> ");
		if (!line || !ft_strncmp(line, "exit", 5))
			break ;
		if (line[0])
			add_history(line);
		free(line);
	}
	return (0);
}