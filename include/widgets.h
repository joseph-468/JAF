#pragma once

#include "color.h"
#include "core.h"

#include <SDL.h>
#include <vector>

namespace JAF {
    class App;
    class Widget;

    struct Event {
        Event() = default;
        virtual ~Event() = default;
        explicit Event(Uint8 type);

        bool unseen{};
        const void *origin{};
        Uint8 type{};
    };

    struct AppEvent final : Event {
        AppEvent() = default;
        explicit AppEvent(const Uint8 type) : Event(type) {}

        enum {
            SDL_EVENT,
        };
        SDL_Event event{};
    };

    struct ButtonEvent final : Event {
        ButtonEvent() = default;
        explicit ButtonEvent(const Uint8 type) : Event(type) {}

        enum {
            PRESSED,
            RELEASED,
        };
    };

    struct CanvasEvent final : Event {
        CanvasEvent() = default;
        explicit CanvasEvent(const Uint8 type) : Event(type) {}

        enum {
            PRESSED,
            RELEASED,
            MOTION,
        };
        Sint32 pressedX{}, pressedY{};
    };

    class Widget {
    public:
        explicit Widget(App *app);
        virtual ~Widget();
        // Called at the start of each update before any events are handled.
        virtual void update(App *app) = 0;
        // Called during the renderWidgets function.
        virtual void display(App *app) = 0;
        // Called after the update method for each event since the last update.
        virtual void handleEvent(App *app, const SDL_Event &event) = 0;

        void addEvent(Event *event) const;

        App *app{};
    };

    class Button final : public Widget {
    public:
        explicit Button(App *app) : Widget(app) {};
        ~Button() override = default;
        void update(App *app) override;
        void display(App *app) override;
        void handleEvent(App *app, const SDL_Event &event) override;

        [[nodiscard]] bool isPressed() const { return pressed; };
        [[nodiscard]] bool isDown() const { return down; }

        static constexpr Uint8 PRESSED = 1;
        static constexpr Uint8 RELEASED = 2;
        Sint32 x{}, y{};
        Sint32 w{}, h{};
        Color color{};

    private:
        bool pressed{};
        bool down{};
    };

    class Canvas final : public Widget {
    public:
        explicit Canvas(App *app) : Widget(app) {};
        ~Canvas() override = default;
        void update(App *app) override {}
        void display(App *app) override;
        void handleEvent(App *app, const SDL_Event &event) override;

        void addTexture(Sint32 textureWidth, Sint32 textureHeight);
        void updateTexture(const std::vector<Uint32> &data) const;

        Sint32 x{}, y{};
        Sint32 w{}, h{};
        Sint32 textureWidth{}, textureHeight{};

    private:
        SDL_Texture *texture{};
        bool down{};
    };
}
