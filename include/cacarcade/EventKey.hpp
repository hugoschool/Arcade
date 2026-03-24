#pragma once

namespace cacarcade {
    enum class EventKey {
        // Nonexistant key
        None,

        // All alphabet keys, by chunks of 9
        A, B, C, D, E, F, G, H, I,
        J, K, L, M, N, O, P, Q, R,
        S, T, U, V, W, X, Y, Z,

        // All number keys
        _1, _2, _3, _4, _5,
        _6, _7, _8, _9, _0,

        // All misc keys (Backspace)
        Backspace,

        // Arrow keys
        Left,
        Right,
        Up,
        Down,
    };
}
