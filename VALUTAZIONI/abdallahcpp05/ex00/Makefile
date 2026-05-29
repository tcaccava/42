# Nom de l'exécutable
NAME = bureaucrat

# Compilateur et flags
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# Fichiers sources et objets
SRCDIR = .
OBJDIR = obj
SOURCES = main.cpp Bureaucrat.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

# Couleurs pour l'affichage
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
RESET = \033[0m

# Règle principale
all: $(NAME)

# Création de l'exécutable
$(NAME): $(OBJECTS)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) created successfully!$(RESET)"

# Création du dossier obj s'il n'existe pas
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Compilation des fichiers .cpp en .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)

# Nettoyage complet
fclean: clean
	@echo "$(RED)Cleaning executable...$(RESET)"
	@rm -f $(NAME)

# Recompilation complète
re: fclean all

# Test après compilation
test: $(NAME)
	@echo "$(GREEN)Running tests...$(RESET)"
	@./$(NAME)

# Pour débugger avec des infos supplémentaires
debug: CXXFLAGS += -g -fsanitize=address
debug: re

# Règles qui ne correspondent pas à des fichiers
.PHONY: all clean fclean re test debug
