NAME			=	minishell

CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -Iincludes

LIBFT_DIR		=	lib/libft
LIBFT			=	$(LIBFT_DIR)/libft.a

SRC_DIR			=	src
PARSING_DIR		=	$(SRC_DIR)/parsing
BUILTINS_DIR	=	$(SRC_DIR)/builtins
HELPERS_DIR		=	$(SRC_DIR)/helpers
EXEC_DIR		=	$(SRC_DIR)/execution

SRCS			=	$(SRC_DIR)/minishell.c $(SRC_DIR)/minishell_utils.c  $(SRC_DIR)/gc.c $(SRC_DIR)/debug.c \
					$(PARSING_DIR)/lexer.c $(PARSING_DIR)/parser.c $(PARSING_DIR)/env.c \
					$(PARSING_DIR)/utils.c $(PARSING_DIR)/heredoc.c $(PARSING_DIR)/signals.c \
					$(PARSING_DIR)/expand.c $(PARSING_DIR)/env_utils.c $(PARSING_DIR)/lexer_utils.c $(PARSING_DIR)/token_quotes.c\
					$(PARSING_DIR)/parser_utils.c $(PARSING_DIR)/free.c $(PARSING_DIR)/setup_signals.c $(PARSING_DIR)/export_utils.c\
					$(PARSING_DIR)/expand_utils.c $(PARSING_DIR)/process_heredoc.c \
					$(PARSING_DIR)/export.c $(PARSING_DIR)/expand_helpers.c\
					$(BUILTINS_DIR)/env.c $(BUILTINS_DIR)/echo.c $(BUILTINS_DIR)/exit.c $(BUILTINS_DIR)/builtins_utils.c \
					$(BUILTINS_DIR)/builtins_utils2.c $(BUILTINS_DIR)/pwd.c $(BUILTINS_DIR)/unset.c \
					$(BUILTINS_DIR)/cd.c $(BUILTINS_DIR)/export.c $(BUILTINS_DIR)/print_export.c $(BUILTINS_DIR)/builtins_helpers.c \
					$(EXEC_DIR)/exec.c $(EXEC_DIR)/set_cmd_path.c $(EXEC_DIR)/utils.c $(EXEC_DIR)/utils2.c $(EXEC_DIR)/fork_fail.c \
					$(EXEC_DIR)/exec_multiple.c $(EXEC_DIR)/exec_single.c $(EXEC_DIR)/handle_redirections.c \
					$(HELPERS_DIR)/atol.c $(HELPERS_DIR)/gc_strdup.c $(HELPERS_DIR)/gc_strjoin.c \
					$(HELPERS_DIR)/gc_split.c $(HELPERS_DIR)/print_err.c $(HELPERS_DIR)/gc_substr.c $(HELPERS_DIR)/ft_strncpy.c\
					$(HELPERS_DIR)/gc_itoa.c $(HELPERS_DIR)/utils.c $(HELPERS_DIR)/gc_split_char.c $(HELPERS_DIR)/split_utils.c
OBJS			=	$(SRCS:.c=.o)

all: $(LIBFT_DIR) $(NAME)

$(LIBFT_DIR):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) includes/minishell.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

src/%.o: src/%.c $(LIBFT)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	@make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: $(LIBFT_DIR)
