/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:32:45 by mberila           #+#    #+#             */
/*   Updated: 2025/06/04 14:28:16 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*rand_str(t_gcnode **gc)
{
	static int	count;
	void		*ptr;

	if (count >= 1024)
		return (NULL);
	ptr = gc_malloc(gc, 1);
	if (!ptr)
		return (NULL);
	count++;
	return (ptr_to_hex_str(gc, ptr));
}

int	open_heredoc(t_gcnode **gc, int *fd)
{
	char	*hd_name;

	hd_name = rand_str(gc);
	unlink(hd_name);
	fd[1] = open(hd_name, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd[1] < 0)
		return (perror("cannot open here_doc file"), 1);
	fd[0] = open(hd_name, O_RDONLY);
	if (fd[0] < 0)
		return (perror("cannot open here_doc file"),
			close(fd[1]), unlink(hd_name), 1);
	unlink(hd_name);
	return (0);
}

void	cleanup(t_gcnode **gc, int *hd_fd, char *delim, int dupped_in)
{
	dup2(dupped_in, STDIN_FILENO);
	close(hd_fd[0]);
	close(hd_fd[1]);
	gc_free(gc, delim);
	exit_status(130, 1);
}

int	heredoc_loop(char *del, char *delim, int hd_out, t_data *data)
{
	char	*line;
	char	*expanded_str;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line || g_sigint_received || equal(line, delim))
		{
			if (!line)
				print_err("warning: delimited by EOF (wanted '%s')\n", del);
			free(line);
			break ;
		}
		if (del[0] == '\'' || del[0] == '\"')
			expanded_str = line;
		else
			expanded_str = expand_variables(data, line);
		ft_putstr_fd(expanded_str, hd_out);
		write(hd_out, "\n", 1);
		if (expanded_str != line)
			gc_free(&data->gc, expanded_str);
		free(line);
	}
	return (g_sigint_received);
}

int	handle_herdoc(t_gcnode **gc, char *del, int *hd_in, t_data *data)
{
	char	*quoted_delim;
	int		hd_fd[2];
	int		og_stdin;

	og_stdin = dup(STDIN_FILENO);
	if (open_heredoc(gc, hd_fd))
		return (1);
	*hd_in = hd_fd[0];
	quoted_delim = remove_quotes(gc, del);
	setup_heredoc_signals();
	data->in_heredoc = 1;
	g_sigint_received = 0;
	if (heredoc_loop(del, quoted_delim, hd_fd[1], data))
		return (cleanup(gc, hd_fd, quoted_delim, og_stdin), 1);
	dup2(og_stdin, STDIN_FILENO);
	setup_interactive_signals();
	close(hd_fd[1]);
	gc_free(gc, quoted_delim);
	return (0);
}
