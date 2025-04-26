/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/04/26 10:49:06 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	f(void)
// {
// 	// system("leaks minishell");
// 	system("lsof | grep '^minishell'");
// }

// static void	print_header()
// {
// 	printf("\n███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     ");
// 	printf("\n████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     ");
// 	printf("\n██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     ");
// 	printf("\n██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     ");
// 	printf("\n██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗");
// 	printf("\n╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n\n" );
// }

int main(int ac, char *av[], char **envp)
{
	t_token		*tokens;
	t_data		*data;
	char		*line;

	data = NULL;
	(void)ac;
	(void)av;
	// atexit(f);
	data = malloc(sizeof(t_data));
	if (!data)
		return (printf("Failed to allocat"), 0);
	data->env = init_env(envp);
	data->exit_status = 0;
	// print_header();
	while (1)
	{
		if (data->exit_status == 0)
			printf( "✔ " );
		else
			printf( "✘ " );
		line = readline("minishell ➤ ");
		if (!line)
			break ;
		if (line[0])
			add_history(line);
		tokens = tokenize(line);
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
			continue ;
		}
		set_cmd_path(data->cmds, data->env);

		// ! ======[ DEBUG: ]======
		// print_tokens(tokens);
		// print_cmds(data->cmds);
		// ! ======================
		
		exec(data);

		free_tokens(tokens);
		free_commands(data->cmds);
		data->cmds = NULL;
		free(line);
	}
	clear_history();
	free_env(data->env);
	free(data);
	return (0);
}
