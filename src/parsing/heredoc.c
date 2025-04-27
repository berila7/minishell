/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:32:45 by mberila           #+#    #+#             */
/*   Updated: 2025/04/27 14:21:25 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	// unlink("here_doc");
	return (0);
}

int handle_herdoc(char *del, int *hd_in, t_data *data)
{
	char	*expanded_str;
	char	*line;
	char    *quoted_delim;
	int		hd_fd[2];

	if (open_heredoc(hd_fd))
		return (1);
	*hd_in = hd_fd[0];
	quoted_delim = remove_quotes(del);
	line = readline("> ");
	while (line)
	{
		if (line[0] == '\'' || line[0] == '\"')
			expanded_str = expand_variables(line, data);
		else
			expanded_str = line;
		if (equal(line, remove_quotes(del)))
		{
			free(line);
			if (expanded_str != line)
				free(expanded_str);
			break ;
		}
		ft_putstr_fd(expanded_str, hd_fd[1]);
		write(hd_fd[1], "\n", 1);
		if (expanded_str != line)
            free(expanded_str);
		free(line);
		line = readline("> ");
	}
	return (free(line), 0);
}
