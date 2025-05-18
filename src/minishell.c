/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/05/18 12:47:12 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	f(void)
// {
// 	// system("leaks minishell");
// 	system("sh -c 'echo \"====================================================\" && lsof -c minishell && echo \"====================================================\"'");
// }

static void	print_header()
{
	printf("\n███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     ");
	printf("\n████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     ");
	printf("\n██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     ");
	printf("\n██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     ");
	printf("\n██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗");
	printf("\n╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n\n" );
}

int main(int ac, char *av[], char **envp)
{
	t_token				*tokens;
	t_data				*data;
	char				*line;

	data = NULL;
	(void)ac;
	(void)av;
	
	// atexit(f);
	// ac = N;
	data = malloc(sizeof(t_data));
	if (!data)
		return (print_err("1- Allocation Error [%d]\n", &ac), 0);
	data->env = init_env(envp);
	if (!data->env)
		return (print_err("2- Allocation Error [%d]\n", &ac), 0);
	setup_interactive_signals();
	print_header();
	while (1)
	{
		if (exit_status(0, 0) == 0)
			printf( "✔ " );
		else
			printf( "✘ " );
		setup_interactive_signals();
		line = readline("minishell ➤ ");
		if (!line)
    	{
        	if (g_sigint_received)
       		{
           		g_sigint_received = 0;
            	continue;
        	}
			else
			{
				printf("exit\n");
				break;
			}
    	}
		if (line[0])
			add_history(line);
		tokens = tokenize(line, data);
		if (!tokens || !validate_token(tokens))
		{
			free_tokens(tokens);
			free(line);
			continue ;
		}
		data->cmds = parse_tokens(tokens, data);
		if (!data->cmds)
		{
			free_tokens(tokens);
			free(line);
			g_sigint_received = 0;
			continue ;
		}
		if (set_cmd_path(data->cmds, data->env))
		{
			free_data(data);
			exit(1);
		}

		// ! ======[ DEBUG: ]======
		print_tokens(tokens);
		print_cmds(data->cmds);
		// ! ======================
		
		setup_exec_signals();
		
		exec(data);
		
		
		free_tokens(tokens);
		free_commands(data->cmds);
		data->cmds = NULL;
		free(line);
		g_sigint_received = 0;
	}
	clear_history();
	free_env(data->env);
	free(data);
	return (0);
}
