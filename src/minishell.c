/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:24:47 by ayoub             #+#    #+#             */
/*   Updated: 2025/05/27 13:09:32 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_line(t_data *data, char *line, t_token **tokens)
{
	*tokens = tokenize(line, data);
	if (!*tokens || !validate_token(*tokens))
		return (0);
	data->cmds = parse_tokens(*tokens, data);
	if (!data->cmds)
	{
		g_sigint_received = 0;
		return (0);
	}
	set_cmd_path(&data->gc, data->cmds, data->env);
	print_tokens(*tokens);
	print_cmds(data->cmds);
	return (1);
}

void	execute_commands(t_data *data)
{
	if (data->cmds)
		exec(data);
	setup_interactive_signals();
	g_sigint_received = 0;
	data->hered_count = 0;
}

void	cleanup_iteration(t_data *data, t_token *tokens, char *line)
{
	free_tokens(&data->gc, tokens);
	free_commands(&data->gc, data->cmds);
	data->cmds = NULL;
	free(line);
}

void	cleanup_and_exit(t_data *data)
{
	reset_to_system_default_signals();
	clear_history();
	gc_free_all(&data->gc);
}

int	main(int ac, char *av[], char **envp)
{
	t_data	*data;

	(void)ac;
	(void)av;
	if (!init_data(&data, envp))
		return (0);
	run_shell_loop(data);
	cleanup_and_exit(data);
	return (0);
}
