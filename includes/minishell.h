/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:12:54 by mberila           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/11 16:17:38 by anachat          ###   ########.fr       */
=======
/*   Updated: 2025/04/11 15:28:06 by mberila          ###   ########.fr       */
>>>>>>> 653f42d1a43289914018cafbdf67710c3804392b
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
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

#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define PURPLE 		"\033[38;2;193;77;255m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

typedef struct s_token t_token;
typedef struct s_command t_command;
typedef struct s_env t_env;

typedef enum e_token_type
{
	TOKEN_WORD,			//	(commands, arguments, filenames)
	TOKEN_PIPE,			//	|
	TOKEN_REDIR_IN,		//	<
	TOKEN_REDIR_OUT,	//	>
	TOKEN_REDIR_APPEND,	//	>>
	TOKEN_HEREDOC		//	<<
}	t_token_type;

struct s_command
{
	char        **args;
	char        *input_file;
	char        *output_file;
	int         append_mode;
	char        *heredoc_delim;
	t_command   *next;
};

struct s_token
{
	char			*value;
	t_token_type	type;
	t_token			*next;
};

<<<<<<< HEAD
t_token *tokenize(char *line);
void	free_tokens(t_token *tokens);
long    ft_atol(const char *str);
int     count_args(char **args);
=======
struct s_env
{
	char		*key;
	char		*value;
	t_env		*next;
};

t_token		*tokenize(char *line);
void		free_tokens(t_token *tokens);
t_command	*parse_tokens(t_token *tokens);
void		free_commands(t_command *commands);
void		free_command(t_command *cmd);
t_env		*init_env(char	**envp);
int			ft_env(t_env *env);
>>>>>>> 653f42d1a43289914018cafbdf67710c3804392b

#endif