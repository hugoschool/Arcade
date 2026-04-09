# Creating a new library

This document describes how to creating a new {graphical, game} library for the Arcade.

## Creating a new Game library

> Replace `<game_name>` with your own game name (ex: minesweeper)

Given that you're respecting the current architecture of our Arcade (fork):

- Create a new folder inside the `src/games` folder with your game name: `src/games/<game_name>`
- Create a new folder inside the `include/games` folder with your game name: `include/games/<game_name>`
- Add your game to the Makefile in a similar manner:
```diff
+<GAME_NAME>_SRC	:=	$(COMMON_SRC) \
+					<GAME_NAME>/entrypoint.cpp \
+					<GAME_NAME>/<GAME_NAME>.cpp

+<GAME_NAME>_OBJ	:=	$(<GAME_NAME>_SRC:.cpp=.o)
+<GAME_NAME>_BIN	:=	$(LIB_PATH)/arcade_<game_name>.so

+ all:	$(MINESWEEPER_BIN) $(CENTIPEDE_BIN) $(<GAME_NAME>_BIN)

+ $(<GAME_NAME>_BIN):	$(<GAME_NAME>_OBJ)

+clean:
+	$(RM) $(<GAME_NAME>_OBJ)

+fclean:	clean
+	$(RM) $(<GAME_NAME>_BIN)
```
- Once done, create a file `src/games/<game_name>/entrypoint.cpp` containing an entry point to your class similar to this:
```cpp
#include "cacarcade/IGameModule.hpp"
#include "games/<game_name>/<game_name>.hpp"

extern "C" cacarcade::IGameModule *gameEntrypoint()
{
    return new arcade::<game_name>Game;
}
```
- Create a file `include/games/<game_name>/<game_name>.hpp` which has a class that must inherit from `arcade::AGameModule` (This is an example, feel free to modify and add more):
```cpp
#pragma once

#include "games/AGameModule.hpp"

namespace arcade {
    class <GAME_NAME>Game : public AGameModule {
        public:
            <GAME_NAME>Game();
            ~<GAME_NAME>Game();

            void update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event) override;
            void reset() override;

        protected:
            void handleEvent(std::unique_ptr<cacarcade::IEvent> &event) override;
    }
}
```
- Create a file `src/games/<game_name>/<game_name>.cpp` where you implement all of the functions written above.

Tip: in the class constructor, you must set the `_container.dimension` to the amount of tiles your game might need.

> It's not obligated to override / implement `handleEvent` if you directly handle them in the `update` method.
>
> The method is only here to provide a way to cleanly separate updating from handling the events.

If done correctly, you should be having a running game in no time.

## Creating a new Graphical library

> Replace `<display_name>` with your own display name (ex: SDL2)

Given that you're respecting the current architecture of our Arcade (fork):

- Create a new folder inside the `src/graphicals` folder with your game name: `src/graphicals/<display_name>`
- Create a new folder inside the `include/graphicals` folder with your game name: `include/graphicals/<display_name>`
- Add your game to the Makefile in a similar manner:
```diff
+<DISPLAY_NAME>_SRC	:=	$(COMMON_SRC) \
+					<DISPLAY_NAME>/entrypoint.cpp \
+					<DISPLAY_NAME>/<DISPLAY_NAME>.cpp

+<DISPLAY_NAME>_OBJ	:=	$(<DISPLAY_NAME>_SRC:.cpp=.o)
+<DISPLAY_NAME>_BIN	:=	$(LIB_PATH)/arcade_<display_name>.so

+ all:	$(SDL2_BIN) $(SFML_BIN) $(NCURSES_BIN) $(<DISPLAY_NAME>_BIN)

+ $(<DISPLAY_NAME>_BIN):	LDLIBS	+=	<related libs>
+ $(<DISPLAY_NAME>_BIN):	$(<DISPLAY_NAME>_OBJ)

+clean:
+	$(RM) $(<DISPLAY_NAME>_OBJ)

+fclean:	clean
+	$(RM) $(<DISPLAY_NAME>_BIN)
```
- Replace the `<related libs>` with whatever libs are needed for your program to compile.
- Once done, create a file `src/graphicals/<display_name>/entrypoint.cpp` containing an entry point to your class similar to this:
```cpp
#include "cacarcade/IDisplayModule.hpp"
#include "graphicals/<DISPLAY_NAME>/<DISPLAY_NAME>.hpp"

extern "C" cacarcade::IDisplayModule *displayEntrypoint() {
    return new arcade::<DISPLAY_NAME>Display;
}
```
- Create a file `include/graphicals/<display_name>/<display_name>.hpp` which has a class that must inherit from `arcade::ADisplayModule`:
```cpp
#pragma once

#include "graphicals/ADisplayModule.hpp"

namespace arcade {
    class <DISPLAY_NAME>Display : public ADisplayModule {
        public:
            <DISPLAY_NAME>Display();
            ~<DISPLAY_NAME>Display();

            void clear() override;

            std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() override;
            void displayTiles(cacarcade::TileContainer tiles) override;
            virtual void displayText(cacarcade::DisplayTextContent) override;
    };
}
```
- Create a file `src/graphicals/<display_name>/<display_name>.cpp` where you implement all of the functions written above.

The methods are quite self descriptive:
- clear: must be used to clear the screen
- pollEvent: must be used to get the events from the display library and reinterpret them as `cacarcade::IEvent`
- displayTiles: must be used display all the tiles from the game onto the display
- displayText: must be used to display text onto the screen
