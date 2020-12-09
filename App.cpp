#include <iostream>
using std::cout;

#include <cmath>

#include "App.hpp"

using namespace Snake;

int main() {
    App app = App();
    return app.Exectute();
}

namespace Snake {

bool App::onInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow(
        "Snake", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 900, 600,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        return false;
    }

    renderer =
        SDL_CreateRenderer(window, -1,
                           SDL_RENDERER_ACCELERATED |
                               SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);

    SDL_FillRect(surface, NULL,
                 SDL_MapRGB(surface->format, 0, 190, 0));
    textures.snake =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FillRect(surface, NULL,
                 SDL_MapRGB(surface->format, 210, 0, 0));
    textures.apple =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FillRect(surface, NULL,
                 SDL_MapRGB(surface->format, 50, 50, 50));
    textures.board =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    onResize(900, 600);

    game.state = new State(20, 20);

    lastMoveTime = SDL_GetTicks();

    return true;
}

void App::onEvent(SDL_Event *event) {
    switch (event->type) {
    case SDL_QUIT:
        appState = QUIT;
        break;
    case SDL_WINDOWEVENT: {
        switch (event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            onResize(event->window.data1,
                     event->window.data2);
            break;
        }
        break;
    }
    case SDL_KEYDOWN: {
        auto itlower =
                 keyMap.lower_bound(event->key.keysym.sym),
             itupper =
                 keyMap.upper_bound(event->key.keysym.sym);
        for (auto it = itlower; it != itupper; ++it) {
            onKey(it->second);
        }
        break;
    }
    }
}

void App::onLoop() {
    switch (appState) {
    case MENU: {
        appState = PLAY;
        break;
    }
    case PLAY: {
        unsigned int currentTime = SDL_GetTicks();
        if (currentTime > lastMoveTime + game.updateTime) {
            if (game.state->CanMove(game.dir)) {
                game.state->Move(game.dir);
            } else {
                appState = DEATH;
                cout << "You lose!\n";
            }
            lastMoveTime = currentTime;
        }
        break;
    }
    case DEATH: {
        break;
    }
    default:
        break;
    }
}

void App::onRender() {
    SDL_RenderClear(renderer);

    SDL_Rect src{0, 0, 1, 1}, dst;

    SDL_RenderCopy(renderer, textures.board, &src,
                   &layout.board);

    for (int x = 0; x < game.state->WIDTH; ++x) {
        for (int y = 0; y < game.state->HEIGHT; ++y) {
            dst.h = layout.vertSize;
            dst.w = layout.vertSize;
            dst.x =
                x * (layout.vertSize + layout.vertPadding) +
                layout.widgetPadding + layout.vertPadding;
            dst.y =
                (game.state->HEIGHT - y - 1) *
                    (layout.vertSize + layout.vertPadding) +
                layout.widgetPadding + layout.vertPadding;

            if (game.state->point(x, y) ==
                game.state->apple) {
                SDL_RenderCopy(renderer, textures.apple,
                               &src, &dst);
            } else if (game.state->point(x, y) ==
                       game.state->head) {
                SDL_RenderCopy(renderer, textures.snake,
                               &src, &dst);
            } else if (game.state->Occupied(x, y) >= 1) {
                Direction dir = game.state->Dir(x, y);
                SDL_RenderCopy(renderer, textures.snake,
                               &src, &dst);
                dst.x += dir.x() * layout.vertPadding;
                dst.y -= dir.y() * layout.vertPadding;
                SDL_RenderCopy(renderer, textures.snake,
                               &src, &dst);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void App::onResize(int width, int height) {

    int maxWidth = width - layout.widgetPadding * 2;
    int maxHeight = height - layout.widgetPadding * 2;

    if (maxWidth / layout.widthVerts <
        maxHeight / layout.heightVerts) {
        layout.vertSize =
            (maxWidth -
             layout.vertPadding * (layout.widthVerts + 1)) /
            layout.widthVerts;
    } else {
        layout.vertSize =
            (maxHeight - layout.vertPadding *
                             (layout.heightVerts + 1)) /
            layout.heightVerts;
    }

    layout.board.w =
        layout.vertSize * layout.widthVerts +
        layout.vertPadding * (layout.widthVerts + 1);
    layout.board.h =
        layout.vertSize * layout.heightVerts +
        layout.vertPadding * (layout.heightVerts + 1);
    layout.board.x = layout.widgetPadding;
    layout.board.y = layout.widgetPadding;
}

void App::onCleanup() {
    SDL_DestroyTexture(textures.snake);
    SDL_DestroyTexture(textures.apple);
    textures = Textures{};

    // might need to deallocate game.engine and game.state

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    SDL_Quit();
}

void App::onKey(KeyFunction key) {
    switch (key) {
    case APPK_NORTH:
        onKeyDir(Direction::NORTH);
        break;
    case APPK_SOUTH:
        onKeyDir(Direction::SOUTH);
        break;
    case APPK_EAST:
        onKeyDir(Direction::EAST);
        break;
    case APPK_WEST:
        onKeyDir(Direction::WEST);
        break;
    case APPK_FAST:
        onKeySpeed(true);
        break;
    case APPK_SLOW:
        onKeySpeed(false);
        break;
    default:
        break;
    }
}

void App::onKeyDir(Direction dir) {
    switch (appState) {
    case MENU:
    case PLAY:
        if (game.engine == NULL)
            game.dir = dir;
    default:
        break;
    }
}

void App::onKeySpeed(bool faster) {
    if (faster) {
        game.updateTime = (game.updateTime + 1) / 2;
    } else {
        game.updateTime *= 2;
    }
}

} // namespace Snake