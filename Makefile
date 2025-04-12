NAME			=	minishell

CC				=	cc
FLAGS			=	-Wall -Wextra -Werror -Iincludes

LIBFT_DIR		=	lib/libft
LIBFT			=	$(LIBFT_DIR)/libft.a

SRC_DIR     	=	src
PARSING_DIR		=	$(SRC_DIR)/parsing
BUILTINS_DIR	=	$(SRC_DIR)/builtins
HELPERS_DIR		=	$(SRC_DIR)/helpers
EXECUTION_DIR	=	$(SRC_DIR)/execution

SRCS			=	$(SRC_DIR)/minishell.c $(SRC_DIR)/debug.c \
					$(PARSING_DIR)/lexer.c $(PARSING_DIR)/parser.c $(PARSING_DIR)/env.c \
					$(BUILTINS_DIR)/env.c $(BUILTINS_DIR)/echo.c $(BUILTINS_DIR)/exit.c \
					$(EXECUTION_DIR)/set_cmd_path.c $(EXECUTION_DIR)/utils.c \
					$(HELPERS_DIR)/atol.c
OBJS			=	$(SRCS:.c = .o)

all: $(LIBFT_DIR) $(NAME)

$(LIBFT_DIR):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -lreadline $(LIBFT) -o $(NAME)

src/%.o: src/%.c $(LIBFT) includes/minishell.h
	$(CC) $(FLAGS) -c $< -o $@ 

clean:
	@make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean:
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: $(LIBFT_DIR)