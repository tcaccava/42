NAME = PhoneBook
SRCS = main.cpp

OBJS = ${SRCS:.cpp=.o}
GG = g++
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -std=c++98

%.o: %.cpp
	${CXX} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	${CXX} ${CFLAGS} ${OBJS} -o ${NAME}

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re