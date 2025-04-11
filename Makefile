NAME		=		minishell

CC			=		cc
LIBFT_DIR	=		lib/libft
FLAGS		=		-Wall -Wextra -Werror -Iincludes
LIBFT		=		$(LIBFT_DIR)/libft.a

SRCS		=		src/minishell.c src/parsing/lexer.c src/parsing/parser.c src/parsing/env.c \
					src/builtins/env.c
OBJS		=		$(SRCS:.c = .o)

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