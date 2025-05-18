/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:32:45 by mberila           #+#    #+#             */
/*   Updated: 2025/05/18 18:59:22 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void restore_terminal(void)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // Restore canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int	open_heredoc(int *fd)
{
	unlink("here_doc");
	fd[1] = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd[1] < 0)
		return (perror("cannot open here_doc file"), 1);
	fd[0] = open("here_doc", O_RDONLY);
	if (fd[0] < 0)
		return (perror("cannot open here_doc file"),
			close(fd[1]), unlink("here_doc"), 1);
	unlink("here_doc");
	return (0);
}

int handle_herdoc(t_gcnode **gc, char *del, int *hd_in, t_data *data)
{
    char    *expanded_str;
    char    *line = NULL;
    char    *quoted_delim;
    int     hd_fd[2];
    int dupped_in = dup(STDIN_FILENO);
    
    if (open_heredoc(hd_fd))
        return (1);
    *hd_in = hd_fd[0];
    quoted_delim = remove_quotes(gc, del);

    g_sigint_received = 0;
    setup_heredoc_signals();
    data->in_heredoc = 1;
    while (1)
    {
        line = readline("> ");
        if (g_sigint_received)
        {
            free(line);
            break ;
        }
            
        if (!line || equal(line, quoted_delim))
        {
            free(line);
            break;
        }
            
        if (del[0] == '\'' || del[0] == '\"')
            expanded_str = line;
        else
            expanded_str = expand_variables(gc, line, data);
        
        ft_putstr_fd(expanded_str, hd_fd[1]);
        write(hd_fd[1], "\n", 1);
        if (expanded_str != line)
            free(expanded_str);
        free(line);
        
    }
    
    dup2(dupped_in, STDIN_FILENO);
    
    if (g_sigint_received)
    {
        close(hd_fd[0]);
        close(hd_fd[1]);
        free(quoted_delim);
        
        exit_status(130, 1);
        return (1);
    }
    setup_interactive_signals();
    restore_terminal();
    close(hd_fd[1]);
    free(quoted_delim);
    return (0);
}
