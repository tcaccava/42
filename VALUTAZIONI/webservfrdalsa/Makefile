SERVER_NAME = webserv
NAME = webserv

SRC_DIR = srcs
COMMON_DIR = $(SRC_DIR)/common
INC_DIR = includes
OBJ_DIR = build
BIN_DIR = bin
VAR_DIR = var

SERVER_SRCS = $(wildcard $(COMMON_DIR)/*.cpp) $(SRC_DIR)/server.cpp

SERVER_OBJS = $(SERVER_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -I$(INC_DIR)

RM = rm -f
MKDIR = mkdir -p

SETUP_SCRIPT = scripts/setup_test_dirs.sh

all: $(BIN_DIR)/$(NAME) setup

$(BIN_DIR)/$(NAME): $(SERVER_OBJS)
	@$(MKDIR) $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SERVER_OBJS) -o $@



$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MKDIR) $(dir $@) # Crea la directory necessaria per l'oggetto
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(SERVER_OBJS)
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) -r $(VAR_DIR)
	$(RM) -r $(BIN_DIR)

re: fclean all

setup:
	bash $(SETUP_SCRIPT)

.PHONY: all clean fclean re setup
