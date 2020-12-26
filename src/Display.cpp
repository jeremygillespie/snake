// DEBUG
#include <iostream>

#include "Display.hpp"

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

    SDL_SetRenderDrawColor(renderer, 0, 90, 0, 0);

    SDL_Surface *surface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);

    auto init_texture = [=](Flat_texture *f) {
        SDL_FillRect(
            surface, NULL, SDL_MapRGB(surface->format, f->r, f->g, f->b));
        f->tex = SDL_CreateTextureFromSurface(renderer, surface);
    };

    init_texture(&textures.snake);
    init_texture(&textures.apple);
    init_texture(&textures.head);
    init_texture(&textures.corner_l);
    init_texture(&textures.corner_r);
    init_texture(&textures.board);

    SDL_FreeSurface(surface);

    on_resize(900, 600);

    start_wall();

    return true;
}

void Display::start_wall() {
    state = State::wall;
}

void Display::start_play() {
    state = State::play;

    stats.last_frame_ticks = SDL_GetTicks();
    stats.last_update_ticks = stats.last_frame_ticks;
}

void Display::end_play() {
    state = State::end;

    int mpa = stats.moves_per_apple(2);
    std::cout << "Apples: " << stats.apples << "\n";
    std::cout << "Moves per apple: " << mpa / 100 << "." << mpa % 100 << "\n";
}

void Display::update() {
    switch (state) {
    case State::play:
        update_play();
        break;
    case State::pause:
        update_pause();
        break;
    default:
        break;
    }
    render();
}

void Display::update_play() {
    ++stats.frame_counts.back();

    unsigned frame_dur = SDL_GetTicks() - stats.last_frame_ticks;

    while (stats.accumulator > 0.0f) {

        if (engine->update()) {
            if (graph->can_move(engine->next_move) &&
                graph->length < graph->size) {
                int old_length = graph->length;

                engine->move();

                ++stats.moves;
                ++stats.move_counts.back();

                if (graph->length > old_length) {
                    ++stats.apples;
                }

                if (graph->length == graph->size) {
                    end_play();
                    break;
                }
            } else {
                end_play();
                break;
            }
            stats.accumulator -= stats.move_interval;
        } else if (engine->show_search) {
            stats.accumulator -= stats.move_interval;
        }

        frame_dur = SDL_GetTicks() - stats.last_frame_ticks;
        if (frame_dur > config.max_frame_dur) {
            break;
        }
    }

    if (frame_dur <= config.max_frame_dur) {
        stats.accumulator += frame_dur * 0.001f;
    } else {
        stats.accumulator = 0.0f;
    }

    stats.last_frame_ticks = SDL_GetTicks();

    if (SDL_GetTicks() >= config.update_interval + stats.last_update_ticks) {
        stats.update();

        stats.last_update_ticks = SDL_GetTicks();
    }
}

void Display::update_pause() {
    if (stats.manual_move) {
        if (engine->update()) {
            if (graph->can_move(engine->next_move) &&
                graph->length < graph->size) {
                int old_length = graph->length;

                engine->move();

                ++stats.moves;
                ++stats.move_counts.back();

                if (graph->length > old_length) {
                    ++stats.apples;
                }

                if (graph->length == graph->size) {
                    end_play();
                }
            } else {
                end_play();
            }
        }

        stats.manual_move = false;
    }
}

void Display::render() {

    SDL_RenderClear(renderer);

    int point;
    SDL_Rect src{0, 0, 1, 1}, dst;

    SDL_RenderCopy(renderer, textures.board.tex, &src, &layout.board);

    // render search_path
    if (engine->show_search) {
        point = graph->head;
        for (auto it = engine->search_path.begin();
             it != engine->search_path.end(); ++it) {
            dst = vertex(graph->x(point), graph->y(point));
            SDL_RenderCopy(renderer, textures.head.tex, &src, &dst);
            dst.x += (*it).x() * layout.vert_padding * 3;
            dst.y -= (*it).y() * layout.vert_padding * 3;
            SDL_RenderCopy(renderer, textures.head.tex, &src, &dst);

            point = graph->point(point, *it);
        }
        dst = vertex(graph->x(point), graph->y(point));
        SDL_RenderCopy(renderer, textures.head.tex, &src, &dst);
    }

    // render graph
    for (int x = 0; x < graph->width; ++x) {
        for (int y = 0; y < graph->height; ++y) {
            dst = vertex(x, y);
            point = graph->point(x, y);

            if (graph->walls[point]) {
                break;
            } else if (point == graph->apple) {
                SDL_RenderCopy(renderer, textures.apple.tex, &src, &dst);
            } else if (graph->occupied[point] > 1) {
                Direction dir =
                    graph->incoming[point] + Direction::turn_reverse;
                SDL_RenderCopy(renderer, textures.snake.tex, &src, &dst);
                dst.x += dir.x() * layout.vert_padding * 3;
                dst.y -= dir.y() * layout.vert_padding * 3;
                SDL_RenderCopy(renderer, textures.snake.tex, &src, &dst);
            } else if (graph->occupied[point] == 1) {
                SDL_RenderCopy(renderer, textures.snake.tex, &src, &dst);
            }
        }
    }

    // render corners
    if (engine->show_search) {
        for (int x = 0; x < engine->width; ++x) {
            for (int y = 0; y < engine->height; ++y) {
                dst = corner(x, y);
                point = x * engine->height + y;

                if (engine->corner(point) == Direction::turn_left) {
                    SDL_RenderCopy(renderer, textures.corner_l.tex, &src, &dst);
                } else if (engine->corner(point) == Direction::turn_right) {
                    SDL_RenderCopy(renderer, textures.corner_r.tex, &src, &dst);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}

SDL_Rect Display::vertex(int x, int y) {
    SDL_Rect result;
    result.h = layout.vert_size - layout.vert_padding * 2;
    result.w = layout.vert_size - layout.vert_padding * 2;
    result.x =
        x * layout.vert_size + layout.vert_padding * 2 + layout.board_padding;
    result.y = (graph->height - y - 1) * layout.vert_size +
               layout.vert_padding * 2 + layout.board_padding;
    return result;
}

SDL_Rect Display::corner(int x, int y) {
    SDL_Rect result;

    result.h = layout.vert_padding * 2;
    result.w = layout.vert_padding * 2;

    result.x = x * layout.vert_size + layout.board_padding;
    result.y =
        (engine->height - y - 1) * layout.vert_size + layout.board_padding;

    return result;
}

void Display::on_event(SDL_Event *event) {
    switch (event->type) {

    case SDL_MOUSEBUTTONDOWN:
        break;

    case SDL_KEYDOWN:
        switch (event->key.keysym.sym) {
        case SDLK_w:
            on_dir(Direction::north);
            break;
        case SDLK_a:
            on_dir(Direction::west);
            break;
        case SDLK_s:
            on_dir(Direction::south);
            break;
        case SDLK_d:
            on_dir(Direction::east);
            break;
        case SDLK_UP:
            on_dir(Direction::north);
            break;
        case SDLK_LEFT:
            on_dir(Direction::west);
            break;
        case SDLK_DOWN:
            on_dir(Direction::south);
            break;
        case SDLK_RIGHT:
            on_dir(Direction::east);
            break;
        case SDLK_SPACE:
            stats.manual_move = true;
            break;
        case SDLK_EQUALS:
            stats.move_interval /= 2;
            break;
        case SDLK_MINUS:
            stats.move_interval *= 2;
            break;
        case SDLK_RETURN:
            switch (state) {
            case State::wall:
                start_play();
                break;
            case State::play:
                state = State::pause;
                break;
            case State::pause:
                state = State::play;
                break;
            case State::end:
                state = State::quit;
                break;
            default:
                break;
            }
        }
        break;

    case SDL_WINDOWEVENT:
        switch (event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            on_resize(event->window.data1, event->window.data2);
            break;
        }
        break;

    case SDL_QUIT:
        state = State::quit;
        break;
    }
}

void Display::on_click(int x, int y) {}

void Display::on_dir(Direction dir) {
    if (dir != graph->incoming[graph->head] + Direction::turn_reverse)
        engine->set_move(dir);
}

void Display::on_resize(int width, int height) {

    if (width / graph->width < height / graph->height) {
        layout.board_padding = width / 20;
        layout.vert_size =
            (width - layout.board_padding * 5 / 2) / graph->width;
    } else {
        layout.board_padding = height / 20;
        layout.vert_size =
            (height - layout.board_padding * 5 / 2) / graph->height;
    }

    layout.vert_padding = layout.vert_size / 5;

    layout.board.w = layout.vert_padding * 2 + graph->width * layout.vert_size;
    layout.board.h = layout.vert_padding * 2 + graph->height * layout.vert_size;
    layout.board.x = layout.board_padding;
    layout.board.y = layout.board_padding;
}

void Display::terminate() {

    for (Flat_texture f :
         {textures.snake, textures.apple, textures.head, textures.corner_l,
          textures.corner_r, textures.board}) {
        SDL_DestroyTexture(f.tex);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

} // namespace snake