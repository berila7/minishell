/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:48 by anachat           #+#    #+#             */
/*   Updated: 2025/05/02 12:01:01 by mberila          ###   ########.fr       */
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
        printf(RED "Args Count: [%d]\n" RESET, count_args(current_cmd->args));

        for (int i = 0; current_cmd->args && current_cmd->args[i]; i++)
        {
            printf(GREEN "  args[%d]: " YELLOW "'%s'\n" RESET, i, current_cmd->args[i]);
        }

        printf(CYAN "  path          : " YELLOW "'%s'\n" RESET, current_cmd->path);
        printf(CYAN "  herdoc fd     : " YELLOW "'%d'\n" RESET, current_cmd->hd_fd);
        
        // Print the original input/output properties for backwards compatibility
        // printf(CYAN "  input file    : " YELLOW "'%s'\n" RESET, current_cmd->input_file ? current_cmd->input_file : "(null)");
        // printf(CYAN "  output file   : " YELLOW "'%s'\n" RESET, current_cmd->output_file ? current_cmd->output_file : "(null)");
        // printf(CYAN "  append mode   : " YELLOW "'%d'\n" RESET, current_cmd->append_mode);
        // printf(CYAN "  heredoc delim : " YELLOW "'%s'\n" RESET, current_cmd->heredoc_delim ? current_cmd->heredoc_delim : "(null)");
        
        // Print all redirections in order
        if (current_cmd->redirections && current_cmd->redir_count > 0)
        {
            printf(GREEN "  Redirections   : \n" RESET);
            for (int i = 0; i < current_cmd->redir_count; i++)
            {
                printf(GREEN "    [%d] Type: " YELLOW, i);
                
                // Print the type in human-readable form
                switch (current_cmd->redirections[i].type)
                {
                    case REDIR_IN:
                        printf("INPUT (<)");
                        break;
                    case REDIR_OUT:
                        printf("OUTPUT (>)");
                        break;
                    case REDIR_APPEND:
                        printf("APPEND (>>)");
                        break;
                    case REDIR_HEREDOC:
                        printf("HEREDOC (<<)");
                        break;
                    default:
                        printf("UNKNOWN (%d)", current_cmd->redirections[i].type);
                }
                
                printf(RESET GREEN ", File: " YELLOW "'%s'\n" RESET, 
                       current_cmd->redirections[i].file ? 
                       current_cmd->redirections[i].file : "(null)");
            }
        }
        else
        {
            printf(GREEN "  Redirections   : " YELLOW "(none)\n" RESET);
        }

        printf(PURPLE "----------------------------\n" RESET);
        current_cmd = current_cmd->next;
    }
    printf(PURPLE "=============================\n" RESET);
    printf("\n" RESET);
}


void	check_fds_in_child(const char *msg)
{
	pid_t pid = getpid();
	char cmd[256];

	fprintf(stderr, "\033[38;5;226m" "%s (PID: %d)\n" "\033[0m", msg, pid);  // Print to stderr instead of stdout
	snprintf(cmd, sizeof(cmd), "lsof -p %d >&2", pid); // Redirect output to stderr
	system(cmd);
}

hi