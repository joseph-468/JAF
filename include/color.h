#pragma once

#include <SDL.h>

namespace JAF {
    struct Color {
        Color() = default;
        Color(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a = 255)
            : r(r), g(g), b(b), a(a) {}
        [[nodiscard]] Uint32 data() const {
            // Byte order reversed because SDL textures are little endian.
            return (a << 24) + (b << 16) + (g << 8) + (r << 0);
        };

        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;
    };
}
