/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:48:06 by mberila           #+#    #+#             */
/*   Updated: 2025/06/28 16:46:31 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_dir(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		print_err("%s: error retrieving current directory\n", "minishell");
		perror("getcwd");
		data->cwd_failed = 1;
	}
	else
		free(cwd);
}

int	init_data(t_data **data, char **envp)
{
	t_gcnode	*gc;

	if (!isatty(STDIN_FILENO))
		return (print_err("STDIN is not a valid tty\n", NULL), 0);
	gc = NULL;
	rl_catch_signals = 0;
	*data = gc_malloc(&gc, sizeof(t_data));
	check_dir(*data);
	(*data)->gc = gc;
	(*data)->hered_count = 0;
	(*data)->is_export = 0;
	(*data)->expandable = 1;
	(*data)->env = init_env(&(*data)->gc, envp);
	if (!env_exists((*data)->env, "PATH"))
		set_env(&(*data)->gc, &(*data)->env, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	set_default_env(*data);
	setup_interactive_signals();
	return (1);
}

void	cleanup_iteration(t_data *data, t_token *tokens, char *line)
{
	free(line);
	free_tokens(&data->gc, tokens);
	free_commands(&data->gc, data->cmds);
	data->expandable = 1;
	data->is_export = 0;
	data->hered_count = 0;
	data->cmds = NULL;
}

void	run_shell_loop(t_data *data)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		line = readline("minishell ➤ ");
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (line[0])
			add_history(line);
		if (!process_line(data, line, &tokens))
		{
			cleanup_iteration(data, tokens, line);
			continue ;
		}
		execute_commands(data);
		cleanup_iteration(data, tokens, line);
	}
}
