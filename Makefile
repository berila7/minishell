NAME		=		minishell

CC			=		cc
LIBFT_DIR	=		lib/libft
PRINTF_DIR	=		lib/ft_printf
FLAGS		=		-Wall -Wextra -Werror
LIBFT		=		$(LIBFT_DIR)/libft.a
PRINTF		=		$(PRINTF_DIR)/libftprintf.a

SRCS		=		src/minishell.c src/parsing/lexer.c src/parsing/parser.c
OBJS		=		$(SRCS:.c = .o)

all: $(LIBFT_DIR) $(PRINTF_DIR) $(NAME)

$(LIBFT_DIR):
	@make -C $(LIBFT_DIR)

$(PRINTF_DIR):
	@make -C $(PRINTF_DIR)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -lreadline $(LIBFT) $(PRINTF) -o $(NAME)

src/%.o: src/%.c $(LIBFT) $(PRINTF) includes/minishell.h
	$(CC) $(FLAGS) -c $< -o $@ 

clean:
	@make -C $(LIBFT_DIR) clean
	@make -C $(PRINTF_DIR) clean
	rm -f $(OBJS)

fclean:
	@make -C $(LIBFT_DIR) fclean
	@make -C $(PRINTF_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: $(LIBFT_DIR) $(PRINTF_DIR)