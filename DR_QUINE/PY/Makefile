NAME_COLLEEN = Colleen
NAME_GRACE   = Grace
NAME_SULLY   = Sully

PYTHON = python3

all: $(NAME_COLLEEN) $(NAME_GRACE) $(NAME_SULLY)

$(NAME_COLLEEN): Colleen.py
	chmod +x Colleen.py
	cp Colleen.py $(NAME_COLLEEN)

$(NAME_GRACE): Grace.py
	chmod +x Grace.py
	cp Grace.py $(NAME_GRACE)

$(NAME_SULLY): Sully.py
	chmod +x Sully.py
	cp Sully.py $(NAME_SULLY)

diff: all
	@echo "=== TEST COLLEEN (PY)==="
	./$(NAME_COLLEEN) > tmp_colleen
	diff -U 3 Colleen.py tmp_colleen && echo "Colleen OK!" || echo "Colleen KO!"
	@rm -f tmp_colleen

	@echo "\n=== TEST GRACE (PY)==="
	./$(NAME_GRACE)
	diff -U 3 Grace.py Grace_kid.py && echo "Grace OK!" || echo "Grace KO!"

	@echo "\n=== TEST SULLY (PY)==="
	./$(NAME_SULLY)
	@diff -q Sully.py Sully_5.py > /dev/null && echo "Sully vs Sully_5 OK" || echo "Sully KO"
	@for i in 5 4 3 2 1; do \
		echo "--- Diff Sully_$$i.py vs Sully_$$(($$i-1)).py ---"; \
		diff Sully_$$i.py Sully_$$(($$i-1)).py | grep -E "^[<>]"; \
	done

clean:
	rm -rf __pycache__ *.pyc

fclean: clean
	rm -f $(NAME_COLLEEN) $(NAME_GRACE) $(NAME_SULLY)
	rm -f Grace_kid.py Sully_*.py

re: fclean all

.PHONY: all clean fclean re