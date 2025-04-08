/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:12:54 by mberila           #+#    #+#             */
/*   Updated: 2025/04/08 13:00:45 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h> 
# include <signal.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"

typedef struct s_token t_token;

typedef enum e_token_type
{
	TOKEN_WORD,			//	(commands, arguments, filenames)
    TOKEN_PIPE,			//	|
    TOKEN_REDIR_IN,		//	<
    TOKEN_REDIR_OUT,	//	>
    TOKEN_REDIR_APPEND,	//	>>
    TOKEN_HEREDOC		//	<<
}	t_token_type;

struct s_token
{
	char			*value;
	t_token_type	type;
	t_token			*next;
};

#endif