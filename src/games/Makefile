COMMON_SRC	:=	AGameModule.cpp \
				ScoreHandler.cpp

MINESWEEPER_SRC	:=	$(COMMON_SRC) \
					minesweeper/entrypoint.cpp \
					minesweeper/Minesweeper.cpp

CENTIPEDE_SRC	:=	$(COMMON_SRC) \
					centipede/entrypoint.cpp \
					centipede/Centipede.cpp

MINESWEEPER_OBJ	:=	$(MINESWEEPER_SRC:.cpp=.o)
MINESWEEPER_BIN	:=	$(LIB_PATH)/arcade_minesweeper.so

CENTIPEDE_OBJ	:=	$(CENTIPEDE_SRC:.cpp=.o)
CENTIPEDE_BIN	:=	$(LIB_PATH)/arcade_centipede.so

include $(BASE_DIR)/common.mk

all:	$(MINESWEEPER_BIN) $(CENTIPEDE_BIN)

$(MINESWEEPER_BIN):	$(MINESWEEPER_OBJ)

$(CENTIPEDE_BIN):	$(CENTIPEDE_OBJ)

clean:
	$(RM) $(CENTIPEDE_OBJ)
	$(RM) $(MINESWEEPER_OBJ)

fclean:	clean
	$(RM) $(CENTIPEDE_BIN)
	$(RM) $(MINESWEEPER_BIN)

re:	fclean all

.PHONY:	all clean fclean re
