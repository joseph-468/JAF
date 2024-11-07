#pragma once

#include "color.h"
#include "core.h"

#include <SDL.h>

namespace JAF {
    class App;

    class Widget {
    public:
        virtual ~Widget() = default;
        virtual void display(App *app) = 0;
        virtual void handleEvent(App *app, const SDL_Event &event) = 0;

        App *app{};
    };

    class Button final : public Widget {
    public:
        Button() = default;
        ~Button() override = default;
        void display(App *app) override;
        void handleEvent(App *app, const SDL_Event &event) override;

        Sint32 x{}, y{};
        Sint32 w{}, h{};
        bool pressed{};
        Color color{};
    };

    class Canvas final : public Widget {
    public:
        Canvas() = default;
        ~Canvas() override = default;
        void display(App *app) override;
        void handleEvent(App *app, const SDL_Event &event) override;

        void addTexture(Sint32 textureWidth, Sint32 textureHeight);

        Sint32 x{}, y{};
        Sint32 w{}, h{};

    private:
        SDL_Texture *texture{};
    };
}
