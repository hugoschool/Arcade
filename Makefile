CXX	:=	clang++
CXXFLAGS	:=	-Wall -Wextra -std=c++20
CPPFLAGS	:=	-I include

ifeq ($(ENV), dev)
	CXXFLAGS	+=	-g3
endif

SRC	:=	src/main.cpp

OBJ	:=	$(SRC:.cpp=.o)

BINARY	:=	arcade

all:	$(BINARY)

$(BINARY):	$(OBJ)
	$(CXX) -o $(BINARY) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(BINARY)

re:	fclean all

.PHONY:	all clean fclean re
