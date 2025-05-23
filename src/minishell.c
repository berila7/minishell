/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/23 14:45:16 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */





#include "minishell.h"

// void	f(void)
// {
// 	// system("leaks minishell");
// 	system("sh -c 'echo \"====================================================\" && lsof -c minishell && echo \"====================================================\"'");
// }

void check_dir()
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		print_err("%s: error retrieving current directory\n", "minishell");
		perror("getcwd");
	}
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
	check_dir();
	gc = NULL;
	data = gc_malloc(&gc, sizeof(t_data));
	data->gc = gc;
	data->hered_count = 0;
	data->env = init_env(&gc, envp);
	if (!env_exists(data->env, "PATH"))
		set_env(&gc, &data->env, "PATH", "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	set_default_env(data);
	setup_interactive_signals();
	while (1)
	{
		line = readline("minishell ➤ ");
		if (!line)
    	{
        	if (isatty(STDIN_FILENO) && g_sigint_received) { // Check if Ctrl+C in heredoc
                 g_sigint_received = 0;
                 // setup_interactive_signals(); // Ensure interactive signals are back
                write(STDOUT_FILENO, "\n", 1); // Or let handler do this
                 // rl_on_new_line(); // May need to ensure readline state is good
                 // rl_redisplay(); 
                 continue;
            } else if (isatty(STDIN_FILENO)) { // Check for TTY to print "exit"
                 write(STDOUT_FILENO, "exit\n", 5);
            }
            break;
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
		// print_tokens(tokens);
		// print_cmds(data->cmds);
		// ! ======================
				
		if (data->cmds)
			exec(data);
		
		setup_interactive_signals();
		g_sigint_received = 0;
		data->hered_count = 0;
		free_tokens(&gc, tokens);
		free_commands(&gc, data->cmds);
		data->cmds = NULL;
		gc_free(&gc, line);
	}
	reset_to_system_default_signals();
	clear_history();
	gc_free_all(&gc);
	return (0);
}
