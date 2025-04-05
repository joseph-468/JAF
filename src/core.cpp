#include <sdl.h>
#define CloseWindow RaylibCloseWindow
#define ShowCursor RaylibShowCursor
#include <raylib.h>
#undef CloseWindow
#undef ShowCursor
#include <spdlog/spdlog.h>

#include "core.h"


void doStuff() {
    InitWindow(600, 600, "Testing");

    SDL_Init(SDL_INIT_VIDEO);
    const void *const windowHandle = GetWindowHandle();
    const SDL_Window *window = SDL_CreateWindowFrom(windowHandle);

    while (!WindowShouldClose()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            spdlog::info("Event type: " + event.type);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(0, 0);
        EndDrawing();
    }

    RaylibCloseWindow();
}