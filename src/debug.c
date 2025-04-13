/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:48 by anachat           #+#    #+#             */
/*   Updated: 2025/04/13 17:11:49 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_tokens(t_token *tokens)
{
	t_token *current = tokens;

	printf(ORANGE "\n========== TOKENS ==========\n" RESET);
	while (current)
	{
		printf(GREEN "Token: " YELLOW "'%s'" RESET ", ", current->value);
		printf(CYAN "Type: %d\n" RESET, current->type);
		current = current->next;
	}
	printf(ORANGE "============================\n" RESET);
	printf("\n" RESET);
}

void print_cmds(t_cmd *cmds)
{
	t_cmd *current_cmd = cmds;
	int j = 0;

	printf(PURPLE "\n========== COMMANDS ==========\n" RESET);
	while (current_cmd)
	{
		j++;
		printf(BLUE "Command: [%d]\n" RESET, j);

		for (int i = 0; current_cmd->args && current_cmd->args[i]; i++)
		{
			printf(GREEN "  args[%d]: " YELLOW "'%s'\n" RESET, i, current_cmd->args[i]);
		}

		printf(CYAN "  path          : " YELLOW "'%s'\n" RESET, current_cmd->path);
		printf(CYAN "  input file    : " YELLOW "'%s'\n" RESET, current_cmd->input_file ? current_cmd->input_file : "(null)");
		printf(CYAN "  output file   : " YELLOW "'%s'\n" RESET, current_cmd->output_file ? current_cmd->output_file : "(null)");
		printf(CYAN "  append mode   : " YELLOW "'%d'\n" RESET, current_cmd->append_mode);
		printf(CYAN "  heredoc delim : " YELLOW "'%s'\n" RESET, current_cmd->heredoc_delim ? current_cmd->heredoc_delim : "(null)");

		printf(PURPLE "----------------------------\n" RESET);
		current_cmd = current_cmd->next;
	}
	printf(PURPLE "=============================\n" RESET);
	printf("\n" RESET);
}