#pragma once

namespace cacarcade {
    enum class EventType {
        KeyPressed,
        TileClicked,

        PrevGame,
        NextGame,

        PrevDisplay,
        NextDisplay,

        Reset,
        Quit,
        Menu
    };
}
