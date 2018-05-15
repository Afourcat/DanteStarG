##
## EPITECH PROJECT, 2017
## Makefile
## File description:
## makefile of the source file
##

ROOT		=	.

SRC_DIR		=	sources

INC_DIR		=	includes

CXX 	    	=	g++

NAME		=	graphical

REAL		=	$(ROOT)/release

SRC 	    	=	$(SRC_DIR)/main.cpp	\

SRC_H		=	$(INC_DIR)/Maze.hpp	\
 	   	    	$(INC_DIR)/App.hpp

WARN		=	-W -Wall -Wextra

CXXFLAGS	=	$(WARN) -I $(REAL) -I $(INC_DIR)

LDFLAGS		=	-l sfml-graphics -l sfml-audio -l sfml-system -l sfml-window

DEB		?=      0

G		=	-g

V		?=	@

OBJS		=	$(patsubst $(SRC_DIR)/%.cpp, $(REAL)/%.o, $(SRC))

OBJS_H		=	$(patsubst $(INC_DIR)/%.hpp, $(REAL)/%.hpp.gch, $(SRC_H))


#_-_-_-_-_-TEST-_-_-_-_-_-_-_-_#

ifeq ($(DEB), 1)
	CXXFLAGS += $(G)
endif

ifeq ($(TEST), 1)
	CXXFLAGS += $(MACRO)
endif

#_-_-_-_-_-_COLOR_-_-_-_-_-_-_-_#

GREEN	=	\e[1;32m

WHITE	=	\e[0m

ORANGE	=	\e[1;33m

RED	=	\e[1;35m

BLUE	=	\e[1;34m

#################################

debug:	CXXFLAGS += $(G)

#################################

all:			$(NAME)

debug:			fclean $(NAME)

$(NAME):		$(REAL) $(OBJS_H) $(OBJS) 
			$(V)printf "$(GREEN)Compile sources.$(WHITE)\n"
			$(V)$(CXX) -o $(NAME) $(OBJS) $(LDFLAGS)
			$(V)printf "$(GREEN)Linking obj and Libraries.$(WHITE)\n"

$(REAL)/%.o:		$(SRC_DIR)/%.cpp
			$(V)printf "$(BLUE)Compiling [$(GREEN)$(notdir $<)$(BLUE) -> $(RED)$(notdir $@)$(BLUE)]\n$(WHITE)"
			$(V)$(CXX) -o $@ -c $< $(CXXFLAGS) $(LDFLAGS)

$(REAL)/%.hpp.gch:	$(INC_DIR)/%.hpp
			$(V)printf "$(BLUE)Compiling [$(GREEN)$(notdir $<)$(BLUE) -> $(RED)$(notdir $@)$(BLUE)]\n$(WHITE)"
			$(V)$(CXX) -o $@ -c $< $(CXXFLAGS) $(LDFLAGS)

clean:
			$(V)rm -rf $(OBJS)
			$(V)printf "$(ORANGE)Removing object files.$(WHITE)\n"

fclean:			clean
			$(V)rm -f $(REAL)/$(NAME)
			$(V)rm -rf $(OBJS) $(OBJS_H)
			$(V)rm -f $(NAME)
			$(V)printf "$(ORANGE)Removing binary file.$(WHITE)\n"

$(REAL):
			$(call make-repo)

re:			fclean
			$(V)make --no-print-directory all

echo_build:		echo_debug
			$(V)printf "$(GREEN)Begin of the build !\n$(ORANGE)Warnings : \n$(WHITE)"

echo_debug:
			$(V)if [ $(DEB) == 1 ]; then\
				printf "$(RED)Debug mode initialized.$(WHITE)\n";\
			fi

.PHONY: clean fclean debug all re echo_build echo_debug

define make-repo
	$(V)for dir in $(dir $(OBJS)); \
	do 			  \
		mkdir -p $$dir;	  \
	done
endef
