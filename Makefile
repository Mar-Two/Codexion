CC = cc
NAME = codexion
CFLAGS = -Wall -Wextra -Werror -pthread -g
SRC = main.c parsing.c init_structures.c threads.c threads2.c time.c heap.c monitor.c init_threads.c
OBJS = $(SRC:.c=.o)
RM = rm -f

all: $(NAME)

%.o: %.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): header.h

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean: 
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
