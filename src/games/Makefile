COMMON_SRC	:=	AGameModule.cpp

MINESWEEPER_SRC	:=	$(COMMON_SRC) \
					minesweeper/main.cpp \
					minesweeper/Minesweeper.cpp

MINESWEEPER_OBJ	:=	$(MINESWEEPER_SRC:.cpp=.o)
MINESWEEPER_BIN	:=	$(LIB_PATH)/arcade_minesweeper.so

include $(BASE_DIR)/common.mk

all:	$(MINESWEEPER_BIN)

$(MINESWEEPER_BIN):	$(MINESWEEPER_OBJ)

clean:
	$(RM) $(MINESWEEPER_OBJ)

fclean:	clean
	$(RM) $(MINESWEEPER_BIN)

re:	fclean all

.PHONY:	all clean fclean re
