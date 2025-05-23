/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/23 15:59:51 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	init_data(t_data **data, char **envp)
{
	t_gcnode	*gc;

	if (!isatty(STDIN_FILENO))
		return (print_err("STDIN is not a valid tty\n", NULL), 0);
	check_dir();
	rl_catch_signals = 0;
	gc = NULL;
	*data = gc_malloc(&gc, sizeof(t_data));
	(*data)->gc = gc;
	(*data)->hered_count = 0;
	(*data)->env = init_env(&(*data)->gc, envp);
	if (!env_exists((*data)->env, "PATH"))
		set_env(&(*data)->gc, &(*data)->env, "PATH", "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	set_default_env(*data);
	setup_interactive_signals();
	return (1);
}

int main(int ac, char *av[], char **envp)
{
	t_token		*tokens;
	t_data		*data;
	char		*line;

	(void)ac;
	(void)av;
	if (!init_data(&data, envp))
		return (0);
	while (1)
	{
		line = readline("minishell ➤ ");
		if (!line)
    	{
            write(STDOUT_FILENO, "exit\n", 5);
            break;
    	}
		if (line[0])
			add_history(line);
		tokens = tokenize(line, data);
		if (!tokens || !validate_token(tokens))
		{
			free_tokens(&data->gc, tokens);
			gc_free(&data->gc, line);
			continue ;
		}
		data->cmds = parse_tokens(tokens, data);
		if (!data->cmds)
		{
			free_tokens(&data->gc, tokens);
			gc_free(&data->gc, line);
			g_sigint_received = 0;
			continue ;
		}
		set_cmd_path(&data->gc, data->cmds, data->env);
		// ! ======[ DEBUG: ]======
		// print_tokens(tokens);
		// print_cmds(data->cmds);
		// ! ======================
		
		if (data->cmds)
			exec(data);	
		setup_interactive_signals();
		g_sigint_received = 0;
		data->hered_count = 0;
		free_tokens(&data->gc, tokens);
		free_commands(&data->gc, data->cmds);
		data->cmds = NULL;
		gc_free(&data->gc, line);
	}
	reset_to_system_default_signals();
	clear_history();
	gc_free_all(&data->gc);
	return (0);
}
