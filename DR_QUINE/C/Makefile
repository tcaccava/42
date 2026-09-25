NAME_COLLEEN = Colleen
NAME_GRACE   = Grace
NAME_SULLY   = Sully

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror 

SRC_COLLEEN = Colleen.c
SRC_GRACE   = Grace.c
SRC_SULLY   = Sully.c

OBJ_COLLEEN = $(SRC_COLLEEN:.c=.o)
OBJ_GRACE   = $(SRC_GRACE:.c=.o)
OBJ_SULLY   = $(SRC_SULLY:.c=.o)

all: $(NAME_COLLEEN) $(NAME_GRACE) $(NAME_SULLY)

$(NAME_COLLEEN): $(OBJ_COLLEEN)
	$(CC) $(CFLAGS) $(OBJ_COLLEEN) -o $(NAME_COLLEEN)

$(NAME_GRACE): $(OBJ_GRACE)
	$(CC) $(CFLAGS) $(OBJ_GRACE) -o $(NAME_GRACE)

$(NAME_SULLY): $(OBJ_SULLY)
	$(CC) $(CFLAGS) $(OBJ_SULLY) -o $(NAME_SULLY)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

diff: all
	@echo "=== TEST COLLEEN (C) ==="
	./$(NAME_COLLEEN) > tmp_colleen
	@diff -U 3 Colleen.c tmp_colleen && echo "Colleen OK!" || echo "Colleen KO!"
	@rm -f tmp_colleen

	@echo "\n=== TEST GRACE (C) ==="
	./$(NAME_GRACE)
	@diff -U 3 Grace.c Grace_kid.c && echo "Grace OK!" || echo "Grace KO!"

	@echo "\n=== TEST SULLY (C)==="
	./$(NAME_SULLY)
	@diff -q Sully.c Sully_5.c > /dev/null && echo "Sully vs Sully_5 OK" || echo "Sully KO"
	@for i in 5 4 3 2 1; do \
		j=$$((i - 1)); \
		echo "--- Diff Sully_$$i.c vs Sully_$$j.c ---"; \
		diff Sully_$$i.c Sully_$$j.c | grep -E "^[<>]" || true; \
	done

clean:
	rm -f $(OBJ_COLLEEN) $(OBJ_GRACE) $(OBJ_SULLY)

fclean: clean
	rm -f $(NAME_COLLEEN) $(NAME_GRACE) $(NAME_SULLY)
	rm -f Grace_kid.c Sully_*.c Sully_*

re: fclean all

.PHONY: all clean fclean re