/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:12:54 by mberila           #+#    #+#             */
/*   Updated: 2025/04/14 11:18:20 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
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
#define BLUE        "\033[38;2;70;130;255m"
#define PURPLE 		"\033[38;2;193;77;255m"
#define CYAN        "\033[36m"
#define ORANGE      "\033[38;2;255;165;0m"
#define WHITE       "\033[37m"

typedef struct s_token t_token;
typedef struct s_cmd t_cmd;
typedef struct s_env t_env;
typedef struct s_data t_data;

typedef enum e_token_type
{
	TOKEN_WORD,			//	(commands, arguments, filenames)
	TOKEN_PIPE,			//	|
	TOKEN_REDIR_IN,		//	<
	TOKEN_REDIR_OUT,	//	>
	TOKEN_REDIR_APPEND,	//	>>
	TOKEN_HEREDOC		//	<<
}	t_token_type;

struct s_cmd
{
	char        **args;
	char        *path;
	char        *input_file;
	char        *output_file;
	int         append_mode;
	char        *heredoc_delim;
	t_cmd   *next;
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

struct s_data
{
	t_cmd	*cmds;
	t_env		*env;
	int			exit_status;
	// int			pipe_fd[2];
};

t_token		*tokenize(char *line, t_env *env, int exit_status);
void		free_tokens(t_token *tokens);
t_cmd		*parse_tokens(t_token *tokens);
void		free_commands(t_cmd *commands);
void		free_command(t_cmd *cmd);
void		free_env(t_env *env);
t_env		*init_env(char	**envp);
long	    ft_atol(const char *str);
int     	count_args(char **args);
int			exec(t_data *data);
char		*expand_variables(char *str, t_env *env, int exit_status);
void		set_cmd_path(t_cmd *cmds, t_env *env);
int			equal(char *s1, char *s2);
void		unset_env(t_env **env, char *key);
char		*ft_strjoin_char(char *str, char c);
char		*remove_quotes(char *str);
char		*get_env(t_env *env, char *key);
void		set_env(t_env **env, char *key, char *value);

// builtins:
int			is_builtin(t_cmd *cmd);
int			exec_builtin(t_cmd *cmd, t_data *data);

void		ft_echo(char **args);
int			ft_exit(char **args);
int			ft_env(t_env *env);
void		ft_unset(char **args, t_env *env);
void		ft_pwd();
void		ft_cd(char **args, t_data *data);

// exec utils:
int			exec(t_data *data);
void		free_arr(char **arr);
int			cmd_exists(char *path);
char		*find_cmd(char **paths, char *cmd);
char		*join_path(char *path, char *cmd);
int			count_cmd(t_cmd *cmd);

// Debug functions:
void		print_tokens(t_token *tokens);
void		print_cmds(t_cmd *cmds);



#endif