NAME = scop

CXX = g++
CC = gcc

CXXFLAGS = -Wall -Wextra -Werror
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude

SRC_CPP = src/main.cpp		\
		  src/Input.cpp		\
		  src/Shader.cpp 	\
		  src/Window.cpp	\
		  src/Mesh.cpp		\
		  src/Renderer.cpp	\
		  src/Math/Matrix4.cpp	\
		  src/Math/Vector3.cpp	\
		  src/Math/Vector2.cpp	\
		  src/ObjLoader.cpp	\
		  src/Transform.cpp \
		  src/Texture.cpp

SRC_C = src/glad.c

OBJ_CPP = $(SRC_CPP:.cpp=.o)
OBJ_C = $(SRC_C:.c=.o)

OBJ = $(OBJ_CPP) $(OBJ_C)

LIBS = -lglfw -ldl -lGL

GREEN = \033[0;32m
BLUE = \033[0;34m
CYAN = \033[0;36m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(CYAN)"
	@printf "╔══════════════════════════════╗\n"
	@printf "║ SCOP - LINKING ║\n"
	@printf "╚══════════════════════════════╝\n"
	@printf "$(RESET)"
	@$(CXX) $(OBJ) $(LIBS) -o $(NAME)
	@printf "$(GREEN)✓ SCOP successfully built!$(RESET)\n"

%.o: %.cpp
	@printf "$(BLUE)Compiling %-25s$(RESET)" "$<"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@printf "$(GREEN) ✓$(RESET)\n"

%.o: %.c
	@printf "$(BLUE)Compiling %-25s$(RESET)" "$<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@printf "$(GREEN) ✓$(RESET)\n"

clean:
	@rm -f $(OBJ)
	@printf "$(CYAN)Object files removed.$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(CYAN)Executable removed.$(RESET)\n"

deps:
	@printf "$(CYAN)Checking dependencies...$(RESET)\n"
	@command -v g++ >/dev/null || { echo "g++ is missing"; exit 1; }
	@command -v gcc >/dev/null || { echo "gcc is missing"; exit 1; }
	@command -v make >/dev/null || { echo "make is missing"; exit 1; }
	@pkg-config --exists glfw3 || { echo "GLFW3 is missing"; exit 1; }
	@printf "$(GREEN)✓ All dependencies are installed.$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re deps