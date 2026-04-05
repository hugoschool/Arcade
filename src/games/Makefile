COMMON_SRC	:=	AGameModule.cpp \
				ScoreHandler.cpp

MINESWEEPER_SRC	:=	$(COMMON_SRC) \
					minesweeper/entrypoint.cpp \
					minesweeper/Minesweeper.cpp

CENTIPEDE_SRC	:=	$(COMMON_SRC) \
					centipede/entrypoint.cpp \
					centipede/Centipede.cpp

MENU_SRC	:=	$(COMMON_SRC) \
				menu/entrypoint.cpp \
				menu/Menu.cpp

MINESWEEPER_OBJ	:=	$(MINESWEEPER_SRC:.cpp=.o)
MINESWEEPER_BIN	:=	$(LIB_PATH)/arcade_minesweeper.so

CENTIPEDE_OBJ	:=	$(CENTIPEDE_SRC:.cpp=.o)
CENTIPEDE_BIN	:=	$(LIB_PATH)/arcade_centipede.so

MENU_OBJ	:=	$(MENU_SRC:.cpp=.o)
MENU_BIN	:=	$(LIB_PATH)/arcade_menu.so

include $(BASE_DIR)/common.mk

all:	$(MINESWEEPER_BIN) $(CENTIPEDE_BIN) $(MENU_BIN)

$(MINESWEEPER_BIN):	$(MINESWEEPER_OBJ)

$(CENTIPEDE_BIN):	$(CENTIPEDE_OBJ)

$(MENU_BIN):	$(MENU_OBJ)

clean:
	$(RM) $(CENTIPEDE_OBJ)
	$(RM) $(MINESWEEPER_OBJ)
	$(RM) $(MENU_OBJ)

fclean:	clean
	$(RM) $(CENTIPEDE_BIN)
	$(RM) $(MINESWEEPER_BIN)
	$(RM) $(MENU_BIN)

re:	fclean all

.PHONY:	all clean fclean re
