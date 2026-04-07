#pragma once

namespace cacarcade {
    enum class EventType {
        KeyPressed,
        TileClicked,

        PrevGame,
        NextGame,

        PrevDisplay,
        NextDisplay,

        // Game specific Events
        DisplayText,

        // Menu specific Events
        LaunchFromMenu,

        Reset,
        Quit,
        Menu
    };
}
