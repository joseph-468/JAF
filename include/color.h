#pragma once

#include <SDL.h>

namespace JAF {
    struct Color {
        Color() = default;
        Color(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a = 255)
            : r(r), g(g), b(b), a(a) {}

        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;
    };
}
