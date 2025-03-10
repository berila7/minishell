SRCS = ft_printf.c ft_putchar.c ft_putstr.c ft_puthex_adr.c ft_putnbr.c
OBJS = $(SRCS:.c=.o)
NAME = libftprintf.a
LIBC = ar rcs
CC = cc
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(LIBC) $(NAME) $(OBJS)

%.o: %.c ft_printf.h
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
