NAME = webserv
CXX = c++
CXX_FLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDES = -I config/includes -I logger/includes -I http/includes -I client/includes

HTTP_SRCS = http/src/message/HttpRequest.cpp \
    http/src/message/HttpResponse.cpp \
    http/src/parser/HttpParser.cpp \
    http/src/wire/HttpWireFormat.cpp

CLIENT_SRCS = client/src/Client.cpp \
    client/src/io/SocketIo.cpp \
    client/src/connection/ClientConnection.cpp \
    client/src/handler/CgiHandler.cpp \
    client/src/handler/RequestHandler.cpp \
    client/src/poll/IoPollHub.cpp

SRCS = main.cpp \
    config/src/Config.cpp \
    config/src/Parsing.cpp \
    config/src/parsingUtility.cpp \
    logger/src/Logger.cpp \
    $(HTTP_SRCS) \
    $(CLIENT_SRCS)

OBJS = $(patsubst %.cpp, .objs/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS) | logs
	$(CXX) $(CXX_FLAGS) $(OBJS) -o $(NAME)

logs:
	@mkdir -p logs

.objs/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf .objs

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re logs
