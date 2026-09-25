NAME_COLLEEN = Colleen
NAME_GRACE   = Grace
NAME_SULLY   = Sully

NASM    = nasm
NFLAGS  = -f elf64
CC      = gcc
CFLAGS  = -no-pie

SRC_COLLEEN = Colleen.s
SRC_GRACE   = Grace.s
SRC_SULLY   = Sully.s

OBJ_COLLEEN = $(SRC_COLLEEN:.s=.o)
OBJ_GRACE   = $(SRC_GRACE:.s=.o)
OBJ_SULLY   = $(SRC_SULLY:.s=.o)

all: $(NAME_COLLEEN) $(NAME_GRACE) $(NAME_SULLY)

$(NAME_COLLEEN): $(OBJ_COLLEEN)
	$(CC) $(CFLAGS) $(OBJ_COLLEEN) -o $(NAME_COLLEEN)

$(NAME_GRACE): $(OBJ_GRACE)
	$(CC) $(CFLAGS) $(OBJ_GRACE) -o $(NAME_GRACE)

$(NAME_SULLY): $(OBJ_SULLY)
	$(CC) $(CFLAGS) $(OBJ_SULLY) -o $(NAME_SULLY)

%.o: %.s
	$(NASM) $(NFLAGS) $< -o $@

diff: all
	@echo "=== TEST COLLEEN (ASM) ==="
	./$(NAME_COLLEEN) > tmp_colleen
	@diff -U 3 Colleen.s tmp_colleen && echo "Colleen OK!" || echo "Colleen KO!"
	@rm -f tmp_colleen

	@echo "\n=== TEST GRACE (ASM) ==="
	./$(NAME_GRACE)
	@diff -U 3 Grace.s Grace_kid.s && echo "Grace OK!" || echo "Grace KO!"

	@echo "\n=== TEST SULLY (ASM) ==="
	./$(NAME_SULLY)
	@diff -q Sully.s Sully_5.s > /dev/null && echo "Sully vs Sully_5 OK" || echo "Sully KO"
	@for i in 5 4 3 2 1; do \
		echo "--- Diff Sully_$$i.s vs Sully_$$(($$i-1)).s ---"; \
		diff Sully_$$i.s Sully_$$(($$i-1)).s | grep -E "^[<>]"; \
	done

clean:
	rm -f $(OBJ_COLLEEN) $(OBJ_GRACE) $(OBJ_SULLY)

fclean: clean
	rm -f $(NAME_COLLEEN) $(NAME_GRACE) $(NAME_SULLY)
	rm -f Grace_kid.s Sully_*.s Sully_*

re: fclean all

.PHONY: all clean fclean re