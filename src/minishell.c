/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/04/12 17:09:45 by anachat          ###   ########.fr       */
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

int main(int ac, char *av[], char **envp)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmd;
	t_env		*env;

	(void)ac;
	(void)av;
	// atexit(f);
	env = init_env(envp);
	// ft_env(envp);
	print_header();
	while (1)
	{
		line = readline(BLUE"minishell ➤ "RESET);
		if (!line || !ft_strncmp(line, "exit", 5))
			break ;
		if (line[0])
			add_history(line);
		tokens = tokenize(line, env, 1);
		cmd = parse_tokens(tokens);
		
		// ! ======[ DEBUG: ]======
		print_tokens(tokens);
		print_cmds(cmd);
		// ! ======================
		
		set_cmd_path(cmd, env);
		print_cmds(cmd);
 
		free_tokens(tokens);
		free_commands(cmd);
		free(line);
	}
	return (0);
}