/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/22 20:47:14 by mberila          ###   ########.fr       */
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

	rl_catch_signals = 0;
	if (!isatty(STDIN_FILENO))
		return (print_err("STDIN is not a valid tty\n", NULL), 1);
	gc = NULL;
	data = gc_malloc(&gc, sizeof(t_data));
	data->gc = gc;
	data->hered_count = 0;
	data->env = init_env(&gc, envp);
	if (!env_exists(data->env, "PATH"))
		set_env(&gc, &data->env, "PATH", "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	set_default_env(data);
	setup_interactive_signals();
	print_header();
	while (1)
	{
		printf("env path [%s]\n", getenv("USER"));
		if (exit_status(0, 0) == 0)
			printf( "✔ " );
		else
			printf( "✘ " );
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
		tokens = tokenize(line, data);
		if (!tokens || !validate_token(tokens))
		{
			free_tokens(&gc, tokens);
			gc_free(&gc, line);
			continue ;
		}
		data->cmds = parse_tokens(tokens, data);
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
		setup_interactive_signals();
		data->hered_count = 0;
		free_tokens(&gc, tokens);
		free_commands(&gc, data->cmds);
		data->cmds = NULL;
		gc_free(&gc, line);
		g_sigint_received = 0;
	}
	clear_history();
	gc_free_all(&gc);
	return (0);
}
