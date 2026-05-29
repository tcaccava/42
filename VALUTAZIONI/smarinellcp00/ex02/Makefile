NAME = Ex02
SRCS = Account.cpp tests.cpp

OBJS = ${SRCS:.cpp=.o}
CXX = c++
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