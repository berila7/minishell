/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:48 by anachat           #+#    #+#             */
/*   Updated: 2025/06/16 10:45:30 by mberila          ###   ########.fr       */
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
		printf(CYAN "Quote Type: %d\n" RESET, current->quote_type);
        printf("=====================\n");
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
        
        if (current_cmd->redirections && current_cmd->redir_count > 0)
        {
            printf(GREEN "  Redirections   : \n" RESET);
            for (int i = 0; i < current_cmd->redir_count; i++)
            {
                char *is_quoted;
                if (current_cmd->redirections[i].quoted == 1)
                    is_quoted = "Yes";
                else
                    is_quoted = "No";
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
                
                printf(RESET GREEN ", File: " YELLOW "'%s'" RESET GREEN ", Quoted: " YELLOW "%s\n" RESET, 
                       current_cmd->redirections[i].file ? 
                       current_cmd->redirections[i].file : "(null)", is_quoted);
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

	fprintf(stderr, "\033[38;5;226m" "\n===========> [%s] (PID: %d)\n" "\033[0m", msg, pid);  // Print to stderr instead of stdout
	snprintf(cmd, sizeof(cmd), "lsof -p %d >&2", pid); // Redirect output to stderr
	system(cmd);
}
