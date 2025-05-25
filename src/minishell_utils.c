/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:21:27 by berila            #+#    #+#             */
/*   Updated: 2025/05/24 16:46:21 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_dir(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		data->cwd_failed = 1;
		print_err("%s: error retrieving current directory\n", "minishell");
		perror("getcwd");
	}
	else
		free(cwd);
}

int	init_data(t_data **data, char **envp)
{
	t_gcnode	*gc;

	if (!isatty(STDIN_FILENO))
		return (print_err("STDIN is not a valid tty\n", NULL), 0);
	check_dir(*data);
	rl_catch_signals = 0;
	gc = NULL;
	*data = gc_malloc(&gc, sizeof(t_data));
	(*data)->gc = gc;
	(*data)->hered_count = 0;
	(*data)->is_quoted = 0;
	(*data)->env = init_env(&(*data)->gc, envp);
	// update_shlvl(*data);
	if (!env_exists((*data)->env, "PATH"))
		set_env(&(*data)->gc, &(*data)->env, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	set_default_env(*data);
	setup_interactive_signals();
	return (1);
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
