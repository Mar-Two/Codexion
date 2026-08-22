CC = cc
NAME = codexion
CFLAGS = -Wall -Wextra -Werror -pthread -g
OBJDIR = obj
SRC = main.c parsing.c init_structures.c init_threads.c time.c heap.c monitor.c predicate.c acquisition.c coder_cycle.c routine.c
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
RM = rm -f

all: $(NAME)

$(OBJDIR)/%.o: %.c header.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean: 
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
