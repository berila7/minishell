/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:11 by mberila           #+#    #+#             */
/*   Updated: 2025/04/11 17:01:39 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	f(void)
{
	system("leaks minishell");
}

static void	print_header()
{
	printf(WHITE"\n███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     ");
	printf("\n████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     ");
	printf("\n██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     ");
	printf("\n██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     ");
	printf("\n██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗");
	printf("\n╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n\n" RESET);
}

int main(int ac, char *av[], char **env)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmd;
	t_env		*envp;

	(void)ac;
	(void)av;
	atexit(f);
	envp = init_env(env);
	ft_env(envp);
	print_header();
	while (1)
	{
		line = readline("minishell> ");
		if (!line || !ft_strncmp(line, "exit", 5))
			break ;
		if (line[0])
			add_history(line);
		tokens = tokenize(line);
		cmd = parse_tokens(tokens);
		t_token	*current = tokens;
		t_command *current_cmd = cmd;
		while (current)
		{
			printf("Token: '%s', Type: %d\n", current->value, current->type);
			current = current->next;
		}
		printf("\n-------\n");
		while (current_cmd)
		{
			int i = 0;
			while (current_cmd->args[i])
			{
				printf("(args[%d]-> '%s' )-->", i, current_cmd->args[i]);
				i++;
			}
			printf("\ninput file: '%s'\n", current_cmd->input_file);
			printf("output file: '%s'\n", current_cmd->output_file);
			printf("apped mode: '%d'\n", current_cmd->append_mode);
			printf("heredoc delim: '%s'\n", current_cmd->heredoc_delim);
			printf("-------\n");
			current_cmd = current_cmd->next;
		}
		
		free_tokens(tokens);
		free_commands(cmd);
		free(line);
	}
	return (0);
}