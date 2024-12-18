#include "jaf.h"

#include <cassert>
#include <random>

constexpr auto SCREEN_WIDTH = 512;
constexpr auto SCREEN_HEIGHT = 512;
constexpr auto WINDOW_TITLE = "Example - Falling Sand";


enum class CellType {
    AIR,
    WALL,
    SAND,
    WATER,
};

struct Cell {
    CellType type = CellType::AIR;
    JAF::Color color = { 0, 0, 0};

    static const Cell EMPTY;
};
const Cell Cell::EMPTY = { CellType::AIR, { 0, 0, 0 }};

class World {
public:
    explicit World(const Sint32 width, const Sint32 height) : width(width), height(height) {
        grid.clear();
        grid.resize(width * height);

        texture.clear();
        texture.resize(width * height);
    }

    void setCell(const Sint32 x, const Sint32 y, const Cell &cell) {
        if (x < 0 || y < 0) return;
        if (x >= width || y >= height) return;
        const Sint32 dst = x + y * width;
        grid[dst] = cell;
    }

    void updateCell(const Sint32 x, const Sint32 y) {
        const Sint32 dst = x + y * width;
        if (grid[dst].type == CellType::SAND) {
            if (y < 0 || y > height - 2) {
                return;
            }
            if (grid[dst + width].type == CellType::AIR) {
                grid[dst + width] = grid[dst];
                grid[dst] = Cell::EMPTY;
            }
            else if (grid[dst + width].type == CellType::WATER) {
                Cell temp = grid[dst];
                grid[dst] = grid[dst + width];
                grid[dst + width] = temp;
            }

            else if (x > 0 && grid[dst + width - 1].type == CellType::AIR) {
                grid[dst + width - 1] = grid[dst];
                grid[dst] = Cell::EMPTY;
            }
            else if (x > 0 && grid[dst + width - 1].type == CellType::WATER) {
                Cell temp = grid[dst];
                grid[dst] = grid[dst + width - 1];
                grid[dst + width - 1] = temp;
            }

            else if (x < width - 1 && grid[dst + width + 1].type == CellType::AIR) {
                grid[dst + width + 1] = grid[dst];
                grid[dst] = Cell::EMPTY;
            }
            else if (x < width - 1 && grid[dst + width + 1].type == CellType::WATER) {
                Cell temp = grid[dst];
                grid[dst] = grid[dst + width + 1];
                grid[dst + width + 1] = temp;
            }
        }

        if (grid[dst].type == CellType::WATER) {
            if (y == height - 1) {
            }
            else if (grid[dst + width].type == CellType::AIR) {
                grid[dst + width] = grid[dst];
                grid[dst] = Cell::EMPTY;
            }
            else if (grid[dst + width].type != CellType::AIR) {
                bool left = rand() % 2;
                if (left) {
                    if (x > 0 && grid[dst - 1].type == CellType::AIR) {
                        grid[dst - 1] = grid[dst];
                        grid[dst] = Cell::EMPTY;
                    }
                }
                else {
                    if (x < width - 1 && grid[dst + 1].type == CellType::AIR) {
                        grid[dst + 1] = grid[dst];
                        grid[dst] = Cell::EMPTY;
                    }
                }
            }
        }
    }

    void updateTexture() {
        for (Sint32 i = 0; i < grid.size(); i++) {
            texture[i] = grid[i].color.data();
        }
    }

    Sint32 width{};
    Sint32 height{};
    std::vector<Cell> grid;
    std::vector<Uint32> texture;
};

class App final : public JAF::App {
protected:
    void init() override {
        createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
        setUpdatesPerSecond(tickRate);

        canvas = new JAF::Canvas(this);
        canvas->x = 0;
        canvas->y = 0;
        canvas->w = 512;
        canvas->h = 512;
        canvas->addTexture(world.width, world.height);

    }

    void update() override {
        while (const auto event = dynamic_cast<JAF::AppEvent *>(getEvent(this))) {
            if (event->event.type == SDL_MOUSEWHEEL) {
                if (event->event.wheel.preciseY == 1) {
                    tickRate *= 1.25;
                }
                else {
                    tickRate /= 1.25;
                }
                if (tickRate < 1) tickRate = 1;
                setUpdatesPerSecond(tickRate);
                spdlog::info("Tick Rate: {}", tickRate);
            }
            if (event->event.type == SDL_KEYDOWN) {
                if (event->event.key.keysym.scancode == SDL_SCANCODE_1) {
                    cellType = CellType::SAND;
                }
                if (event->event.key.keysym.scancode == SDL_SCANCODE_2) {
                    cellType = CellType::WATER;
                }
            }
        }

        for (Sint32 y = world.height - 1; y > -1; y--) {
            for (Sint32 x = 0; x < world.width; x++) {
                world.updateCell(x, y);
            }
        }

        bool cellPlaced = false;
        Cell newCell = Cell::EMPTY;
        if (cellType == CellType::SAND) {
            newCell = { CellType::SAND, { 203, 189, 147 } };
        }
        else if (cellType == CellType::WATER) {
            newCell = { CellType::WATER, { 50, 180, 220 } };
        }
        while (const auto event = dynamic_cast<JAF::CanvasEvent *>(getEvent(canvas))) {
            if (event->type == JAF::CanvasEvent::PRESSED) {
                cellPlaced = true;
                clickingCanvas = true;
                world.setCell(cursorX, cursorY, newCell);
            }
            if (event->type == JAF::CanvasEvent::RELEASED) {
                clickingCanvas = false;
            }
            if (event->type == JAF::CanvasEvent::MOTION) {
                cursorX = event->pressedX;
                cursorY = event->pressedY;
                if (clickingCanvas) {
                    world.setCell(cursorX, cursorY, newCell);
                }
            }
        }
        if (clickingCanvas && !cellPlaced) {
            world.setCell(cursorX, cursorY, newCell);
        }
    }

    void render() override {
        world.updateTexture();
        canvas->updateTexture(world.texture);
        renderWidgets();
    }

    void quit() override {}

    static constexpr auto DEFAULT_WORLD_WIDTH = 128;
    static constexpr auto DEFAULT_WORLD_HEIGHT = 128;
    static constexpr auto DEFAULT_TICK_RATE = 64;

    Sint64 ticks{};
    double tickRate = DEFAULT_TICK_RATE;
    bool clickingCanvas{};
    CellType cellType = CellType::SAND;
    World world{ DEFAULT_WORLD_WIDTH, DEFAULT_WORLD_HEIGHT };
    Sint32 cursorX{}, cursorY{};
    JAF::Canvas *canvas{};
};

int main(int argc, char *argv[]) {
    App app;
    app.run();
    return 0;
}
