/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/04/14 11:58:31 by anachat          ###   ########.fr       */
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
	t_token		*tokens;
	t_data		*data;
	char		*line;

	data = NULL;
	(void)ac;
	(void)av;
	atexit(f);
	data = malloc(sizeof(t_data));
	if (!data)
		return (printf("Failed to allocat"), 0);
	data->env = init_env(envp);
	data->exit_status = 1;
	ft_env(data->env);
	print_header();
	while (1)
	{
		line = readline(BLUE"minishell ➤ "RESET);
		if (!line)
		{
			free(data);
			return (printf("exit\n"), 0);
		}
		if (line[0])
			add_history(line);
		tokens = tokenize(line, data->env, data->exit_status);
		data->cmds = parse_tokens(tokens);
		set_cmd_path(data->cmds, data->env);

		// ! ======[ DEBUG: ]======
		print_tokens(tokens);
		print_cmds(data->cmds);
		// ! ======================
		
		// exec(data);
 
		free_tokens(tokens);
		free_commands(data->cmds);
		free(line);
	}
	free_env(data->env);
	free(data);
	return (0);
}
