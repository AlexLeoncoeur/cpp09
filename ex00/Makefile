# COLORS
MAGENTA = \033[35;1m
YELLOW  = \033[33;1m
GREEN   = \033[32;1m
WHITE   = \033[37;1m
RESET   = \033[0m
GRAY 	= \033[0;90m
BLUE    = \033[34;1m
CYAN    = \033[37;1m
BOLD	= \033[1m
RED		= \033[31;1m

#---------- BASE ----------#

# FILES 
CPP_FILES = main.cpp\

CLASS_CPP_FILES =  \
#SRC_FILES = $(shell find $(SRC_DIR) -name '*.cpp')

# DIRECTORIES 
SRC_DIR = src/
OBJ_DIR = objs/

CLASS_DIR = src/classes/
CLASS_OBJ_DIR = objs/classes/

# OBJECTS
CPP_OFILES = $(addprefix $(OBJ_DIR), $(CPP_FILES:.cpp=.o))
CLASS_OFILES = $(addprefix $(CLASS_OBJ_DIR), $(CLASS_CPP_FILES:.cpp=.o))

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@ mkdir -p $(OBJ_DIR)
	@ echo "$(BLUE)-Compiling File: $(CYAN)src/$(RESET)$(notdir $<)"
	@ $(CC) $(FLAGS) -c $< -o $@ -g

$(CLASS_OBJ_DIR)%.o: $(CLASS_DIR)%.cpp
	@ mkdir -p $(CLASS_OBJ_DIR)
	@ echo "$(BLUE)-Compiling File: $(CYAN)src/classes$(RESET)$(notdir $<)"
	@ $(CC) $(FLAGS) -c $< -o $@ -g

#---------- FLAGS & COMPILATION ----------#

CC = c++
NAME = abomination
CFLAGS = -Wall -Werror -Wextra -std=c++98

all: $(NAME)
$(NAME): compiling $(CPP_OFILES) $(CLASS_OFILES)
	@ echo
	@ $(CC) $(CFLAGS) $(CPP_OFILES) $(CLASS_OFILES) -o $(NAME)
	@ echo "$(YELLOW)COMPILATION FINISHED!$(RESET)"

debug: all
	@ echo
	@ echo "$(RED)DEBUG MODE ACTIVATED!$(RESET)"
	@ valgrind -s ./$(NAME)

libft:
	@ make --silent -C include/libft/ bonus

#---------- CLEAN ----------#

clean:
	@ echo "$(RED)CLEANING PROJECT ... $(RESET)"
	@ rm -rf $(OBJ_DIR)
	@ echo "$(YELLOW)PROJECT CLEANED!\n $(RESET)"

fclean: clean
	@ rm -f $(NAME)

re: fclean all

rebug: re debug

compiling:
	@ echo "$(MAGENTA)COMPILING PROJECT: $(RESET)"

.PHONY: all clean fclean re compiling debug rebug