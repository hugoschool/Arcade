COMMON_SRC	:=	ADisplayModule.cpp

SDL2_SRC	:= 	$(COMMON_SRC) \
				SDL2/entrypoint.cpp \
				SDL2/SDL2.cpp

SFML_SRC	:= 	$(COMMON_SRC) \
				SFML/entrypoint.cpp \
				SFML/SFML.cpp

SDL2_OBJ	:=	$(SDL2_SRC:.cpp=.o)
SDL2_BIN	:=	$(LIB_PATH)/arcade_sdl2.so

SFML_OBJ	:=	$(SFML_SRC:.cpp=.o)
SFML_BIN	:=	$(LIB_PATH)/arcade_sfml.so

include $(BASE_DIR)/common.mk

all:	$(SDL2_BIN) $(SFML_BIN)

$(SDL2_BIN):	LDLIBS	+=	-lSDL2 -lSDL2_ttf
$(SDL2_BIN):	$(SDL2_OBJ)

$(SFML_BIN):	LDLIBS	+=	-lsfml-system -lsfml-graphics -lsfml-window
$(SFML_BIN):	$(SFML_OBJ)

clean:
	$(RM) $(SDL2_OBJ)
	$(RM) $(SFML_OBJ)

fclean:	clean
	$(RM) $(SDL2_BIN)
	$(RM) $(SFML_BIN)

re:	fclean all

.PHONY:	all clean fclean re
