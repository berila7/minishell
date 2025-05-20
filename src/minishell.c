/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/05/20 19:34:02 by ayoub            ###   ########.fr       */
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
	t_gcnode	*gc;
	t_token		*tokens;
	t_data		*data;
	char		*line;

	(void)ac;
	(void)av;

	if (!isatty(STDIN_FILENO))
		return (print_err("STDIN is not a valid tty\n", NULL), 1);
	gc = NULL;
	data = gc_malloc(&gc, sizeof(t_data));
	data->gc = gc;
	data->env = init_env(&gc, envp);
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
				break ;
			}
    	}
		if (line[0])
			add_history(line);
		tokens = tokenize(&gc, line, data);
		if (!tokens || !validate_token(tokens))
		{
			free_tokens(&gc, tokens);
			gc_free(&gc, line);
			continue ;
		}
		data->cmds = parse_tokens(&gc, tokens, data);
		if (!data->cmds)
		{
			free_tokens(&gc, tokens);
			gc_free(&gc, line);
			g_sigint_received = 0;
			continue ;
		}
		set_cmd_path(&gc, data->cmds, data->env);
		// ! ======[ DEBUG: ]======
		print_tokens(tokens);
		print_cmds(data->cmds);
		// ! ======================
		
		setup_exec_signals();
		
		exec(data);
		
		
		free_tokens(&gc, tokens);
		free_commands(&gc, data->cmds);
		data->cmds = NULL;
		gc_free(&gc, line);
		g_sigint_received = 0;
	}
	reset_signals();
	clear_history();
	gc_free_all(&gc);
	return (0);
}
