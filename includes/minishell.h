/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:27:17 by anachat           #+#    #+#             */
/*   Updated: 2025/06/25 09:42:47 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define __USE_POSIX
# define _POSIX_C_SOURCE 200809L

# include <sys/stat.h>
# include <signal.h>
# include <fcntl.h>
# include <termios.h>
# include <limits.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h> 
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft.h"

# define RESET			"\033[0m"
# define BLACK			"\033[30m"
# define RED			"\033[31m"
# define GREEN			"\033[32m"
# define YELLOW			"\033[33m"
# define BLUE			"\033[38;2;70;130;255m"
# define PURPLE			"\033[38;2;193;77;255m"
# define CYAN			"\033[36m"
# define ORANGE			"\033[38;2;255;165;0m"
# define WHITE			"\033[37m"

# define SINGLE_QUOTE -1
# define DOUBLE_QUOTE -2

# define REDIR_IN		1
# define REDIR_OUT		2  
# define REDIR_APPEND	3
# define REDIR_HEREDOC	4

typedef struct s_token	t_token;
typedef struct s_cmd	t_cmd;
typedef struct s_env	t_env;
typedef struct s_data	t_data;
typedef struct s_redir	t_redir;
typedef struct s_expand	t_expand;
typedef struct s_gcnode	t_gcnode;
typedef struct s_fdnode	t_fdnode;

typedef struct s_pid_node
{
	pid_t				pid;
	struct s_pid_node	*next;
}	t_pid_node;

struct s_redir
{
	int		type;
	char	*file;
	char	quoted;
};

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

struct s_cmd
{
	char			**args;
	char			*path;
	t_redir			*redirections;
	int				redir_count;
	int				hd_fd;
	t_cmd			*next;
};

struct s_token
{
	char			*value;
	int				quote_type;
	t_token_type	type;
	t_token			*next;
	t_token			*prev;
};

struct s_env
{
	char			*key;
	char			*value;
	t_env			*next;
};

struct s_data
{
	t_cmd			*cmds;
	t_env			*env;
	t_gcnode		*gc;
	t_fdnode		*fd_list;
	t_pid_node		*pids;
	int				pipe[2];
	int				og_fd[2];
	int				hered_count;
	int				in_heredoc;
	int				is_export;
	int				squote_case;
	int				expandable;
	int				cwd_failed;
	int				fork_failed;
};

struct s_gcnode
{
	void			*ptr;
	t_gcnode		*next;
};

struct s_expand
{
	int				in_single_quote;
	int				in_double_quote;
	char			*result;
	t_data			*data;
};

void		*gc_malloc(t_gcnode **gc, size_t size);
void		gc_free_all(t_gcnode **gc);
void		gc_free(t_gcnode **gc, void *ptr);

char		*gc_strjoin(t_gcnode **gc, char *s1, char *s2);
char		**gc_split(t_gcnode **gc, char *s);
char		**gc_split_char(t_gcnode **gc, char *s, char c);
char		*gc_strdup(t_gcnode **gc, char *src);

t_token		*tokenize(char *line, t_data *data);
void		free_tokens(t_gcnode **gc, t_token *tokens);
char		*get_arg(t_gcnode **gc, char *file);
t_cmd		*parse_tokens(t_token *tokens, t_data *data);
void		free_commands(t_gcnode **gc, t_cmd *commands);
void		free_command(t_gcnode **gc, t_cmd *cmd);
void		free_env(t_gcnode **gc, t_env *env);
t_env		*init_env(t_gcnode **gc, char **envp);
long		ft_atol(const char *str);
int			is_ambiguous(t_token *token, char *str);
int			count_args(char **args);
int			exec(t_data *data);
int			gc_word_count_quoted(char *s);
char		*token_value(t_data *data, char *str);
char		*ft_strncpy(char *dest, const char *src, size_t n);
char		*expand_variables(t_data *data, char *str);
void		set_cmd_path(t_gcnode **gc, t_cmd *cmds, t_env *env);
int			equal(char *s1, char *s2);
char		*token_key(t_data *data, char *str);
int			find_word_end(char *s, int start, char *in_quote_char);
int			skip_word_end(char *s, int start, char *in_quote_char);
void		check_quotes(t_token **token);
char		*add_quotes_to_str(t_gcnode **gc, char *str);
void		unset_env(t_gcnode **gc, t_env **env, char *key);
char		*ft_strjoin_char(t_gcnode **gc, char *str, char c);
char		*remove_quotes(t_gcnode **gc, char *str);
char		*get_env(t_env *env, char *key);
void		set_env(t_gcnode **gc, t_env **env, char *key, char *value);
void		free_data(t_gcnode **gc, t_data *data);
int			validate_token(t_token *token);
void		export_exist(t_token **tokens, t_data *data);
char		*ptr_to_hex_str(t_gcnode **gc, void *ptr);
int			open_heredoc(t_gcnode **gc, int *fd);
void		reset_to_system_default_signals(void);
void		setup_child_default_signals(void);
void		setup_parent_waiting_signals(void);
int			handle_single_quote(t_gcnode **gc, char *str, int i, t_expand *exp);
int			handle_double_quote(t_gcnode **gc, char *str, int i, t_expand *exp);
void		print_err(char *fmt, void *arg);
void		cleanup_and_exit(t_data *data);
void		cleanup_iteration(t_data *data, t_token *tokens, char *line);
void		execute_commands(t_data *data);
int			handle_exit_status(t_gcnode **gc, int i, t_expand *exp);
int			handle_named_var(t_gcnode **gc, char *str, int i, t_expand *exp);
int			process_var_value(t_gcnode **gc, char *var_name,
				int i, t_expand *exp);
int			get_var_name_end(char *str, int i);
int			process_line(t_data *data, char *line, t_token **tokens);
int			init_data(t_data **data, char **envp);
void		run_shell_loop(t_data *data);
char		*ft_strjoin_char_free(t_gcnode **gc, char *str, char c);
char		*ft_strjoin_free(t_gcnode **gc, char *s1, char *s2);
void		export_handler(t_token **tokens, t_data *data);
int			toggel_quote(char *line, int *i);
int			handle_quote_error(t_token **tokens, t_data *data, int in_quote);
int			set_redir_type(t_token_type type);
void		add_argument(t_token *token, t_data *data, t_cmd *cmd, char *arg);
int			handle_pipe(t_token **token, t_cmd **current_cmd, t_cmd **cmd_list,
				t_data *data);
int			process_heredoc_token(t_token **token, t_cmd *cmd,
				t_cmd *cmds, t_data *data);
int			process_token(t_token **token, t_cmd **cmd,
				t_cmd **cmds, t_data *data);
int			process_token_word(t_token **token,
				t_cmd *cmd, t_data *data);
int			in_quotes(char *value);
int			is_valid_var_char(char c);
void		add_token(t_token **tokens, t_token *new_token);
t_token		*new_token(t_gcnode **gc, char *value, t_token_type type);
t_cmd		*new_command(t_gcnode **gc);
char		*gc_itoa(t_gcnode **gc, int n);
void		env_append(t_env **env, t_env *new_node);
void		add_redirection(t_gcnode **gc, t_cmd *cmd, int type, char *file);
int			noquotes_len(char *str);
int			process_heredoc_token(t_token **token,
				t_cmd *current_cmd, t_cmd *cmd_list, t_data *data);
void		process_unquoted_token(t_token *token, t_data *data, char *expanded,
				t_cmd *current_cmd);
void		add_command(t_cmd **cmds, t_cmd *new_cmd, t_data *data);
int			exit_status(int status, int update);
char		*gc_substr(t_gcnode **gc, const char *s,
				unsigned int start, size_t len);
void		gc_free(t_gcnode **gc, void *ptr);
void		extract_word(t_token **tokens, char *line, int *i, t_data *data);

void		exec_builtin(t_cmd *cmd, t_data *data, int exit_flag);
int			ft_echo(char **args);
int			ft_exit(char **args, int exit_flag);
int			ft_env(t_cmd *cmd, t_env *env);
int			ft_unset(char **args, t_data *data);
int			ft_pwd(t_data *data);
int			ft_cd(char **args, t_data *data);
int			ft_export(char **args, t_data *data);

int			open_infile(char *path, t_cmd *cmd);
int			open_outfile(char *file, int mode);
int			is_builtin(t_cmd *cmd);
int			is_valid_env_key(char *str);
void		sorted_env(t_gcnode **gc, t_env *env);

int			exec(t_data *data);
int			ft_execve(t_cmd *cmd, t_data *data);
void		free_arr(t_gcnode **gc, char **arr);
int			cmd_exists(char *path);
int			is_exec(char *path);
char		*join_path(t_gcnode **gc, char *path, char *cmd);
int			count_cmd(t_cmd *cmd);
int			ft_dup2(int oldfd, int newfd);
char		**env_to_array(t_gcnode **gc, t_env *env);
int			env_exists(t_env *env, char *key);
int			handle_redirections(t_cmd *cmd, t_data *data);
int			ft_wait(pid_t last_pid, int default_st);
void		dup2_og(t_data *data);
void		dup_og(t_data *data);
void		close2(int *fds);
void		close_pipes(t_data *data);
void		set_default_env(t_data *data);
char		*in_cur_dir(t_gcnode **gc, char *cmd);
char		*get_env_path(t_env *env);

int			exec_single_cmd(t_data *data);
int			exec_multiple_cmd(t_data *data);
int			handle_exec_errors(t_cmd *cmd, t_data *data);
int			handle_herdoc(t_token *token, int *cmd_hd_in, t_data *data);
void		close_hds(t_data *data);
int			is_directory(char *path);
void		add_pid_to_list(t_data *data, pid_t pid);
void		kill_all_pids(t_data *data);
void		clear_all_pids(t_data *data);

extern int				g_sigint_received;
void		setup_interactive_signals(void);
void		setup_heredoc_signals(void);
void		signal_handler_heredoc(int signum);
void		signal_handler_interactive(int signum);

#endif