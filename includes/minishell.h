/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:12:54 by mberila           #+#    #+#             */
/*   Updated: 2025/04/12 12:08:58 by mberila          ###   ########.fr       */
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

struct s_env
{
	char		*key;
	char		*value;
	t_env		*next;
};



t_token		*tokenize(char *line, t_env *env, int exit_status);
void		free_tokens(t_token *tokens);
t_command	*parse_tokens(t_token *tokens);
void		free_commands(t_command *commands);
void		free_command(t_command *cmd);
t_env		*init_env(char	**envp);
long	    ft_atol(const char *str);
int     	count_args(char **args);
char		*expand_variables(char *str, t_env *env, int exit_status);

// builtins:
void		ft_echo(char **args);
int			ft_exit(char **args);
int			ft_env(t_env *env);

#endif