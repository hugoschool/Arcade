SDL2_SRC	:=	SDL2/main.cpp \
				SDL2/SDL2.cpp
SDL2_OBJ	:=	$(SDL2_SRC:.cpp=.o)
SDL2_BIN	:=	$(LIB_PATH)/arcade_sdl2.so

include $(BASE_DIR)/common.mk

all:	$(SDL2_BIN)

$(SDL2_BIN):	LDLIBS	+=	-lSDL2
$(SDL2_BIN):	$(SDL2_OBJ)

clean:
	$(RM) $(SDL2_OBJ)

fclean:	clean
	$(RM) $(SDL2_BIN)

re:	fclean all

.PHONY:	all clean fclean re
