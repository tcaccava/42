NAME 				=	converter

CC					=	c++
CFLAGS				=	-Wall -Wextra -Werror -std=c++98 -g
RM					=	/bin/rm -f

HEADER				=	ScalarConverter.hpp AConverter.hpp ConverterSelector.hpp \
						ConvertDouble.hpp ConvertChar.hpp ConvertInt.hpp ConvertFloat.hpp
SRCS				=	ScalarConverter.cpp AConverter.cpp ConverterSelector.cpp \
						ConvertDouble.cpp ConvertChar.cpp ConvertInt.cpp ConvertFloat.cpp main.cpp

OBJS				=	$(SRCS:.cpp=.o)

%.o: %.cpp
					$(CC) $(CFLAGS) -c $< -o $@

all:				$(NAME)

$(NAME):			$(OBJS) $(HEADER)
					$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
					$(RM) $(OBJS)

fclean:				clean
					$(RM) $(NAME)

re:					fclean all

.PHONY:				all clean fclean re norm
