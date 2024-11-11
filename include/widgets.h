#pragma once

#include "color.h"
#include "core.h"

#include <SDL.h>
#include <vector>
#include <iostream>

namespace JAF {
    class App;

    class Widget {
    public:
        explicit Widget(App *app);
        virtual ~Widget();
        virtual void display(App *app) = 0;
        virtual void handleEvent(App *app, const SDL_Event &event) = 0;

        App *app{};
    };

    class Button final : public Widget {
    public:
        explicit Button(App *app) : Widget(app) {};
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
        explicit Canvas(App *app) : Widget(app) {};
        ~Canvas() override = default;
        void display(App *app) override;
        void handleEvent(App *app, const SDL_Event &event) override;

        void addTexture(Sint32 textureWidth, Sint32 textureHeight);
        void updateTexture(const std::vector<Uint32> &data) const;

        Sint32 x{}, y{};
        Sint32 w{}, h{};
        Sint32 textureWidth{}, textureHeight{};

        bool pressed{};
        Sint32 pressedX{}, pressedY{};

    private:
        SDL_Texture *texture{};
    };
}
