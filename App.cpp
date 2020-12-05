#include <iostream>
using std::cout;

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

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, 900, 600,
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

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 50, 50, 50));
    textures.board = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    onResize(900, 600);

    lastMoveTime = SDL_GetTicks();

    game.state = new State(20, 20);

    return true;
}

void App::onEvent(SDL_Event *event) {
    switch (event->type) {
    case SDL_QUIT:
        appState = QUIT;
        break;
    case SDL_KEYDOWN:
        break;
    case SDL_WINDOWEVENT:
        switch (event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            onResize(event->window.data1, event->window.data2);
            break;
        }
        break;
    }
}

void App::onLoop() {
    unsigned int currentTime = SDL_GetTicks();
    if (currentTime > lastMoveTime + 500) {
        if (game.state->CanMove(Direction{})) {
            game.state->Move(Direction{});
        }
        lastMoveTime = currentTime;
    }
}

void App::onRender() {
    SDL_RenderClear(renderer);

    SDL_Rect src{0, 0, 1, 1}, dst;

    SDL_RenderCopy(renderer, textures.board, &src, &layout.board);

    for (int x = 0; x < game.state->WIDTH; ++x) {
        for (int y = 0; y < game.state->HEIGHT; ++y) {
            dst.h = layout.vertSize;
            dst.w = layout.vertSize;
            dst.x = x * (layout.vertSize + layout.vertPadding) +
                    layout.widgetPadding + layout.vertPadding;
            dst.y = (game.state->HEIGHT - y - 1) *
                        (layout.vertSize + layout.vertPadding) +
                    layout.widgetPadding + layout.vertPadding;

            if (game.state->point(x, y) == game.state->apple) {
                SDL_RenderCopy(renderer, textures.apple, &src, &dst);
            } else if (game.state->Occupied(x, y)) {
                SDL_RenderCopy(renderer, textures.snake, &src, &dst);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void App::onResize(int width, int height) {

    int maxWidth = width - layout.widgetPadding * 2;
    int maxHeight = height - layout.widgetPadding * 2;

    if (maxWidth / layout.widthVerts < maxHeight / layout.heightVerts) {
        layout.vertSize =
            (maxWidth - layout.vertPadding * (layout.widthVerts + 1)) /
            layout.widthVerts;
    } else {
        layout.vertSize =
            (maxHeight - layout.vertPadding * (layout.heightVerts + 1)) /
            layout.heightVerts;
    }

    layout.board.w = layout.vertSize * layout.widthVerts +
                     layout.vertPadding * (layout.widthVerts + 1);
    layout.board.h = layout.vertSize * layout.heightVerts +
                     layout.vertPadding * (layout.heightVerts + 1);
    layout.board.x = layout.widgetPadding;
    layout.board.y = layout.widgetPadding;
}

void App::onCleanup() {
    SDL_DestroyTexture(textures.snake);
    SDL_DestroyTexture(textures.apple);
    textures = Textures{};

    delete game.state;
    delete game.engine;
    game = Game{};

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    SDL_Quit();
}

} // namespace Snake