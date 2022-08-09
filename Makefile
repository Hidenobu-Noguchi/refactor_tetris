NAME = tetris

CFLAGS = -Wall -Wextra -MMD -MP -lncurses

SRCS = tetris.c
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: all clean fclean re

all: $(NAME)
a: all

clean:
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME)

re: fclean all


.PHONY: debug leaks

debug: CFLAGS += -g -fsanitize=address -fsanitize=undefined -fsanitize=integer
debug: re
d: debug

leaks: CFLAGS += -g -DLEAKS
leaks: re
l: leaks

-include $(DEPS)

