/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/16 11:27:19 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H




// static int n = 0;
// #define malloc(x) (n++ == 4 ? NULL : (malloc(x)))





# define __USE_POSIX
# define _POSIX_C_SOURCE 199309L

# include <signal.h>
# include <fcntl.h>
#include <termios.h>
# include <limits.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h> 
# include <string.h>
# include <errno.h>
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

#define REDIR_IN 1
#define REDIR_OUT 2  
#define REDIR_APPEND 3
#define REDIR_HEREDOC 4

typedef struct s_token t_token;
typedef struct s_cmd t_cmd;
typedef struct s_env t_env;
typedef struct s_data t_data;
typedef struct s_redir t_redir;

struct s_redir {
    int type;
    char *file;
};

typedef enum e_token_type
{
	TOKEN_WORD,			//	(commands, arguments, filenames)
	TOKEN_PIPE,			//	|
	TOKEN_REDIR_IN,		//	<
	TOKEN_REDIR_OUT,	//	>
	TOKEN_REDIR_APPEND,	//	>>
	TOKEN_HEREDOC		//	<<
}	t_token_type;

struct s_cmd {
	char	**args;
	char	*path;
	t_redir *redirections;
	int		redir_count;
	int		hd_fd;
	t_cmd	*next;
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
	t_cmd		*cmds;
	t_env		*env;
	int			pipe[2];
	int			og_fd[2];
	int			in_heredoc;
	int			exit_status;
};

t_token		*tokenize(char *line);
void		free_tokens(t_token *tokens);
t_cmd		*parse_tokens(t_token *tokens, t_data *data);
void		free_commands(t_cmd *commands);
void		free_command(t_cmd *cmd);
void		free_env(t_env *env);
t_env		*init_env(char	**envp);
long	    ft_atol(const char *str);
int     	count_args(char **args);
int			exec(t_data *data);
char		*expand_variables(char *str, t_data *data);
void		set_cmd_path(t_cmd *cmds, t_env *env);
int			equal(char *s1, char *s2);
void		unset_env(t_env **env, char *key);
char		*ft_strjoin_char(char *str, char c);
char		*remove_quotes(char *str);
char		*get_env(t_env *env, char *key);
void		set_env(t_env **env, char *key, char *value);
void		free_data(t_data *data);
int			validate_token(t_token *token);
int			open_heredoc(int *fd);
void		print_err(char *fmt, void *arg);
char		*ft_strjoin_char_free(char *str, char c);
char		*ft_strjoin_free(char *s1, char *s2);
char		*word_split_join(char *dest, char *src);
void		add_argument(t_cmd *cmd, char *arg);
void		process_token_word(t_token *token, t_cmd *current_cmd, t_data *data);


// char		*remove_escape_chars(char *str);
void		extract_word(t_token **tokens, char *line, int *i);
// builtins:
int			is_builtin(t_cmd *cmd);
void		exec_builtin(t_cmd *cmd, t_data *data);

void		ft_echo(char **args);
int			ft_exit(char **args, t_data *data);
int			ft_env(t_env *env);
void		ft_unset(char **args, t_env *env);
void		ft_pwd(t_data *data);
void		ft_cd(char **args, t_data *data);
void		ft_export(char **args, t_data *data);

int			open_infile(char *path, t_cmd *cmd);
int			open_outfile(char *file, int mode);
int			is_builtin(t_cmd *cmd);


// exec utils:
int			exec(t_data *data);
void		free_arr(char **arr);
int			cmd_exists(char *path);
int			is_exec(char *path);
char		*find_cmd(char **paths, char *cmd);
char		*join_path(char *path, char *cmd);
int			count_cmd(t_cmd *cmd);
int			ft_dup2(int oldfd, int newfd);
char		**env_to_array(t_env *env);
int			handle_redirections(t_cmd *cmd, t_data *data);
int			ft_wait(pid_t last_pid, int default_st);
void		dup2_og(t_data *data);
void		dup_og(t_data *data);
void		close2(int *fds);
void		close_pipes(t_data *data);

int			exec_single_cmd(t_data *data);
int			exec_multiple_cmd(t_data *data);
int			handle_herdoc(char *end, int *hd_fd, t_data *data);
void		close_hds(t_data *data);


// Debug functions:
void		print_tokens(t_token *tokens);
void		print_cmds(t_cmd *cmds);
void		check_fds_in_child(const char *msg);


// Signal handling
extern volatile sig_atomic_t g_sigint_received;
void	setup_interactive_signals(void);
void	setup_heredoc_signals(void);
void	setup_exec_signals(void);
void	reset_signals(void);
void	signal_handler_heredoc(int signum);
void	signal_handler_interactive(int signum);
void	signal_handler_exec(int signum);

#endif