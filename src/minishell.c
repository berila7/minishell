/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/04/12 14:09:57 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	f(void)
{
	system("leaks minishell");
}

static void	print_header()
{
	printf(RED"\n███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     ");
	printf("\n████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     ");
	printf("\n██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     ");
	printf("\n██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     ");
	printf("\n██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗");
	printf("\n╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n\n" RESET);
}

int main(int ac, char *av[], char **env)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmd;
	t_env		*envp;

	(void)ac;
	(void)av;
	// atexit(f);
	envp = init_env(env);
	// ft_env(envp);
	print_header();
	while (1)
	{
		line = readline(BLUE"minishell ➤ "RESET);
		if (!line || !ft_strncmp(line, "exit", 5))
			break ;
		if (line[0])
			add_history(line);
		tokens = tokenize(line);
		cmd = parse_tokens(tokens);
		t_token	*curr_tokens = tokens;
		t_command *curr_cmd = cmd;
		
		print_tokens(curr_tokens);
		print_cmds(curr_cmd);
		
		free_tokens(tokens);
		free_commands(cmd);
		free(line);
	}
	return (0);
}