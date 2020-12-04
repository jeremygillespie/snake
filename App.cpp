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

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 200, 0));
    textures.snake = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 200, 0, 0));
    textures.apple = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 50, 50, 50));
    textures.widget = SDL_CreateTextureFromSurface(renderer, surface);

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

    SDL_Rect src{0, 0, 1, 1};

    SDL_RenderCopy(renderer, textures.board, &src, &layout.board);
    SDL_RenderCopy(renderer, textures.widget, &src, &layout.widget1);
    SDL_RenderCopy(renderer, textures.widget, &src, &layout.widget2);

    // for (int x = 0; x < game.state->WIDTH; ++x) {
    //     for (int y = 0; y < game.state->HEIGHT; ++y) {
    //         dst.h = squareSize - padding;
    //         dst.w = squareSize - padding;
    //         dst.x = padding + x * squareSize;
    //         dst.y = (game.state->HEIGHT - y - 1) * squareSize + padding * 2;

    //         if (game.state->point(x, y) == game.state->apple) {
    //             SDL_RenderCopy(renderer, textures.apple, &src, &dst);
    //         } else if (game.state->Occupied(x, y)) {
    //             SDL_RenderCopy(renderer, textures.snake, &src, &dst);
    //         }
    //     }
    // }

    SDL_RenderPresent(renderer);
}

void App::onResize(int width, int height) {
    int widgetSize, boardMaxWidth, boardMaxHeight;

    if (width > height) {
        widgetSize = (height - 3 * layout.widgetPadding) / 2;
        boardMaxWidth = width - 3 * layout.widgetPadding - widgetSize;
        boardMaxHeight = height - layout.widgetPadding * 2;

        layout.board.w = boardMaxWidth;
        layout.board.h = boardMaxHeight;
        layout.board.x = widgetSize + 2 * layout.widgetPadding;
        layout.board.y = layout.widgetPadding;

        layout.widget1.w = widgetSize;
        layout.widget1.h = widgetSize;
        layout.widget1.x = layout.widgetPadding;
        layout.widget1.y = layout.widgetPadding;

        layout.widget2.w = widgetSize;
        layout.widget2.h = widgetSize;
        layout.widget2.x = layout.widgetPadding;
        layout.widget2.y = widgetSize + 2 * layout.widgetPadding;

    } else {
        widgetSize = (width - 3 * layout.widgetPadding) / 2;
        boardMaxWidth = width - layout.widgetPadding * 2;
        boardMaxHeight = height - 3 * layout.widgetPadding - widgetSize;

        layout.board.w = boardMaxWidth;
        layout.board.h = boardMaxHeight;
        layout.board.x = layout.widgetPadding;
        layout.board.y = layout.widgetPadding;

        layout.widget1.w = widgetSize;
        layout.widget1.h = widgetSize;
        layout.widget1.x = layout.widgetPadding;
        layout.widget1.y = boardMaxHeight + 2 * layout.widgetPadding;

        layout.widget2.w = widgetSize;
        layout.widget2.h = widgetSize;
        layout.widget2.x = widgetSize + 2 * layout.widgetPadding;
        layout.widget2.y = boardMaxHeight + 2 * layout.widgetPadding;
    }
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