SRC	:=	main.cpp \
		Arcade.cpp

OBJ	:=	$(SRC:.cpp=.o)

BINARY	:=	$(BASE_DIR)/arcade

all:	$(BINARY)

$(BINARY):	$(OBJ)
	$(CXX) -o $(BINARY) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(BINARY)

re:	fclean all

.PHONY:	all clean fclean re
