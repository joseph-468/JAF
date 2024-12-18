#include "widgets.h"

#include <string>
#include <iostream>

namespace JAF {
    Widget::Widget(App *app) {
        this->app = app;
        app->addWidget(this);
    }

    Widget::~Widget() {
        app->removeWidget(this);
    }

    void Widget::addEvent(Event *const event) const {
        event->origin = this;
        event->unseen = true;
        if (app->eventQueuePos >= App::MAX_WIDGET_EVENTS) return;
        app->eventQueue[app->eventQueuePos++] = event;
    }

    Event::Event(const Uint8 type) : type(type) {}

    void Button::update(App *app) {
        pressed = false;
    }

    void Button::display(App *const app) {
        app->drawRectangle(x, y, w, h, color);
    }

    void Button::handleEvent(App *const app, const SDL_Event &event) {
        const SDL_Point mousePos = { app->getMouseX(), app->getMouseY() };
        const SDL_Rect buttonRect = { x, y, w, h };

        if (SDL_PointInRect(&mousePos, &buttonRect) && app->isLeftMousePressed()) {
            const auto event = new ButtonEvent(PRESSED);
            addEvent(event);
            pressed = true;
            down = true;
        }
        else if (app->isLeftMouseDown() && SDL_PointInRect(&mousePos, &buttonRect) && down) {
            pressed = false;
            down = true;
        }
        else {
            pressed = false;
            down = false;
        }
        if (down) {
            color = { 0, 255, 0 };
        }
        else {
            color = { 255, 0, 0, };
        }
    }

    void Canvas::display(App *const app) {
        const SDL_Rect dst = { x, y, w, h };
        SDL_RenderCopy(app->getRenderer(), texture, nullptr, &dst);
    }

    void Canvas::handleEvent(App *const app, const SDL_Event &event) {
        const SDL_Point mousePos = { app->getMouseX(), app->getMouseY() };
        const SDL_Rect canvasRect = { x, y, w, h };
        const auto pressedX = static_cast<Sint32>(static_cast<float>(mousePos.x - x) / (static_cast<float>(w) / static_cast<float>(textureWidth)));
        const auto pressedY = static_cast<Sint32>(static_cast<float>(mousePos.y - y) / (static_cast<float>(h) / static_cast<float>(textureHeight)));

        if (event.type == SDL_MOUSEMOTION) {
            const auto canvasEvent = new CanvasEvent;
            canvasEvent->pressedX = pressedX;
            canvasEvent->pressedY = pressedY;
            canvasEvent->type = CanvasEvent::MOTION;
            addEvent(canvasEvent);
        }
        else if (SDL_PointInRect(&mousePos, &canvasRect)) {
            if (app->isLeftMouseDown() && !down) {
                const auto canvasEvent = new CanvasEvent;
                canvasEvent->pressedX = pressedX;
                canvasEvent->pressedY = pressedY;
                canvasEvent->type = CanvasEvent::PRESSED;
                addEvent(canvasEvent);
            }
            else if (!app->isLeftMouseDown() && down) {
                const auto canvasEvent = new CanvasEvent;
                canvasEvent->pressedX = pressedX;
                canvasEvent->pressedY = pressedY;
                canvasEvent->type = CanvasEvent::RELEASED;
                addEvent(canvasEvent);
            }
            down = app->isLeftMouseDown();
        }
        else if (down) {
            down = false;
            const auto canvasEvent = new CanvasEvent;
            canvasEvent->pressedX = -1;
            canvasEvent->pressedY = -1;
            canvasEvent->type = CanvasEvent::RELEASED;
            addEvent(canvasEvent);
        }
    }


    void Canvas::addTexture(const Sint32 textureWidth, const Sint32 textureHeight) {
        JAF_ASSERT(textureWidth > 0);
        JAF_ASSERT(textureHeight > 0);

        this->textureWidth = textureWidth;
        this->textureHeight = textureHeight;
        texture = SDL_CreateTexture(
            app->getRenderer(),
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            textureWidth, textureHeight
        );
        JAF_ASSERT(texture != nullptr);

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        void *pixels;
        Sint32 pitch;
        SDL_LockTexture(texture, nullptr, &pixels, &pitch);
        std::memset(pixels, 0, textureHeight * pitch);
        SDL_UnlockTexture(texture);
    }

    void Canvas::updateTexture(const std::vector<Uint32> &data) const {
        JAF_ASSERT(texture != nullptr);
        JAF_ASSERT(data.size() == textureWidth * textureHeight);

        Uint32 *pixels;
        Sint32 pitch;
        SDL_LockTexture(texture, nullptr, reinterpret_cast<void **>(&pixels), &pitch);

        for (Sint32 y = 0; y < textureHeight; y++) {
            std::memcpy(
                pixels + y * (pitch / sizeof(Uint32)),
                data.data() + y * textureWidth,
                textureWidth * sizeof(Uint32)
            );
        }

        SDL_UnlockTexture(texture);
    }
}
