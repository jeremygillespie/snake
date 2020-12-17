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
    Engine *engine = NULL;

    if (argc > 1) {
        std::string e = argv[argc - 1];
        if (e.compare("manhattan") == 0) {
            engine = new Manhattan{graph};
        } else if (e.compare("human") == 0) {
            engine = new Engine{graph};
        } else if (e.compare("naive") == 0) {
            engine = new Naive{graph};
        }
    }

    using default_engine = Engine;
    if (engine == NULL)
        engine = new default_engine{graph};

    // execute
    Display display = {engine};
    return display.execute();
}

namespace snake {

int Display::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    state = wall;

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

    SDL_SetRenderDrawColor(renderer, 0, 90, 0, 0);

    SDL_Surface *surface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 170, 0));
    textures.snake = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
    textures.board = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 200, 0, 0));
    textures.apple = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    on_resize(900, 600);

    return true;
}

void Display::update() {
    if (state == play)
        update_play();
    render();
}

void Display::on_event(SDL_Event *event) {
    switch (event->type) {
    case SDL_QUIT:
        state = quit;
        break;
    case SDL_MOUSEBUTTONDOWN:
        break;
    case SDL_WINDOWEVENT:
        switch (event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            on_resize(event->window.data1, event->window.data2);
            break;
        }
        break;
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym) {
        case SDLK_w:
            direction = Direction::north;
            break;
        case SDLK_a:
            direction = Direction::west;
            break;
        case SDLK_s:
            direction = Direction::south;
            break;
        case SDLK_d:
            direction = Direction::east;
            break;
        case SDLK_UP:
            direction = Direction::north;
            break;
        case SDLK_LEFT:
            direction = Direction::west;
            break;
        case SDLK_DOWN:
            direction = Direction::south;
            break;
        case SDLK_RIGHT:
            direction = Direction::east;
            break;
        case SDLK_EQUALS:
            move_interval += 1;
            move_interval /= 2;
            break;
        case SDLK_MINUS:
            move_interval *= 2;
            break;
        case SDLK_RETURN:
            switch (state) {
            case wall:
                state = play;
                last_move_time = SDL_GetTicks();
                break;
            case end:
                state = quit;
                break;
            default:
                break;
            }
        }
        break;
    }
}

void Display::update_play() {
    unsigned time = SDL_GetTicks();
    if (time >= last_move_time + move_interval) {
        if (engine->update(direction) == false)
            state = end;
        if (engine->graph.length == engine->graph.size)
            state = end;
        last_move_time = time;
    }
}

void Display::render() {

    SDL_RenderClear(renderer);

    SDL_Rect src{0, 0, 1, 1}, dst;

    SDL_RenderCopy(renderer, textures.board, &src, &layout.board);

    for (int x = 0; x < graph->width; ++x) {
        for (int y = 0; y < graph->height; ++y) {

            dst.h = layout.vert_size - layout.vert_padding * 2;
            dst.w = layout.vert_size - layout.vert_padding * 2;
            dst.x = x * layout.vert_size + layout.vert_padding * 2 +
                    layout.board_padding;
            dst.y = (graph->height - y - 1) * layout.vert_size +
                    layout.vert_padding * 2 + layout.board_padding;

            int point = graph->point(x, y);
            if (graph->walls[point]) {
                break;
            } else if (point == graph->apple) {
                SDL_RenderCopy(renderer, textures.apple, &src, &dst);
            } else if (point == graph->head) {
                SDL_RenderCopy(renderer, textures.snake, &src, &dst);
            } else if (graph->occupied[point] >= 1) {
                Direction dir = graph->directions[point];
                SDL_RenderCopy(renderer, textures.snake, &src, &dst);
                dst.x += dir.x() * layout.vert_padding * 3;
                dst.y -= dir.y() * layout.vert_padding * 3;
                SDL_RenderCopy(renderer, textures.snake, &src, &dst);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Display::on_resize(int width, int height) {

    if (width / graph->width < height / graph->height) {
        layout.board_padding = width / 20;
        layout.vert_size = (width - layout.board_padding * 5 / 2) /
                           graph->width;
    } else {
        layout.board_padding = height / 20;
        layout.vert_size = (height - layout.board_padding * 5 / 2) /
                           graph->height;
    }

    layout.vert_padding = layout.vert_size / 10;

    layout.board.w = layout.vert_padding * 2 + graph->width * layout.vert_size;
    layout.board.h = layout.vert_padding * 2 + graph->height * layout.vert_size;
    layout.board.x = layout.board_padding;
    layout.board.y = layout.board_padding;
}

void Display::terminate() {
    SDL_DestroyTexture(textures.snake);
    SDL_DestroyTexture(textures.apple);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

} // namespace snake