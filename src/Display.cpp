#include <chrono>
#include <iostream>
#include <string>

#include "Display.hpp"

using namespace snake;

int main(int argc, char *argv[]) {

    // initialize graph
    int width = 8, height = 8, x = 3, y = 3, length = 4;
    if (argc > 2) {
        width = std::stoi(argv[1]);
        height = std::stoi(argv[2]);
    }
    if (argc > 4) {
        x = std::stoi(argv[3]);
        y = std::stoi(argv[4]);
    }
    if (argc > 5) {
        length = std::stoi(argv[5]);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    Graph graph = {width, height, x, y, length, seed};

    // initialize engine
    Engine *engine;
    if (argc > 1 && std::strcmp(argv[argc - 1], "manhattan") == 0) {
    } else {
        engine = new Engine{graph};
    }

    // execute
    Display display = {engine};
    return display.execute();
}

namespace snake {

int Display::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow(
        "Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 600,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        return false;
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_Surface *surface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 190, 0));
    textures.snake = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 210, 0, 0));
    textures.apple = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    return true;
}

void Display::update() {}

void Display::on_event(SDL_Event *event) {}

void Display::update_wall() {}

void Display::update_play() {}

void Display::render() {

    SDL_RenderClear(renderer);

    SDL_Rect src{0, 0, 1, 1}, dst;

    Graph graph = engine->graph;
    for (int x = 0; x < graph.width; ++x) {
        for (int y = 0; y < graph.height; ++y) {

            dst.h = layout.size;
            dst.w = layout.size;
            dst.x = x * (layout.size + layout.padding) + layout.padding +
                layout.padding;
            dst.y = (graph.height - y - 1) * (layout.size + layout.padding) +
                layout.padding + layout.padding;

            int point = point;
            if (point == graph.apple) {
                SDL_RenderCopy(renderer, textures.apple, &src, &dst);
            } else if (point == graph.head) {
                SDL_RenderCopy(renderer, textures.snake, &src, &dst);
            } else if (graph.occupied[point] >= 1) {
                Direction dir = graph.directions[point];
                SDL_RenderCopy(renderer, textures.snake, &src, &dst);
                dst.x += dir.x() * layout.padding;
                dst.y -= dir.y() * layout.padding;
                SDL_RenderCopy(renderer, textures.snake, &src, &dst);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Display::terminate() {
    SDL_DestroyTexture(textures.snake);
    SDL_DestroyTexture(textures.apple);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

} // namespace snake