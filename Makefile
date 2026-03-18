BASE_DIR	:=	$(realpath .)
LIB_PATH	:=	$(BASE_DIR)/lib

CXX	:=	clang++
CXXFLAGS	:=	-Wall -Wextra -std=c++20
CPPFLAGS	:=	-I $(BASE_DIR)/include/

ifeq ($(ENV), dev)
	CXXFLAGS	+=	-g3
endif

# Make all calls to other makefiles inherit those variables
export BASE_DIR
export LIB_PATH
export CXX
export CXXFLAGS
export CPPFLAGS

# Disable "Entering directory" for every -C option
MAKEFLAGS += --no-print-directory

all:	core games graphicals

.PHONY: core
core:
	$(MAKE) -C src/core

.PHONY: games
games:
	$(MAKE) -C src/games

.PHONY: graphicals
graphicals:
	$(MAKE) -C src/graphicals

clean:
	$(MAKE) -C src/core clean
	$(MAKE) -C src/games clean
	$(MAKE) -C src/graphicals clean

fclean:	clean
	$(MAKE) -C src/core fclean
	$(MAKE) -C src/games fclean
	$(MAKE) -C src/graphicals fclean

re:	fclean all

.PHONY:	all clean fclean re
