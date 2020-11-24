#include "CompactState.hpp"

namespace Snake {

CompactState::CompactState() : board{} {
    length = (HEIGHT + 1) / 2;

    int x = (WIDTH + 1) / 2 - 1;

    head = x * HEIGHT + length - 1;
    tail = x * HEIGHT;

    for (int pos = head; pos > tail; --pos) {
        point(pos, UP & OCCUPIED_MASK);
    }

    point(head, VISITED_MASK);

    firstApple();
}

bool CompactState::canRight() const {
    return head + HEIGHT < SIZE && canMove(point(head + HEIGHT));
}

bool CompactState::canLeft() const {
    return head - HEIGHT >= 0 && canMove(point(head - HEIGHT));
}

bool CompactState::canUp() const {
    return head % HEIGHT < HEIGHT - 1 && canMove(point(head + 1));
}

bool CompactState::canDown() const {
    return head % HEIGHT > 0 && canMove(point(head - 1));
}

CompactState CompactState::right(const CompactState &prev) {}

CompactState CompactState::up(const CompactState &prev) {}

CompactState CompactState::left(const CompactState &prev) {}

CompactState CompactState::down(const CompactState &prev) {}

CompactState CompactState::nextApple(const CompactState &prev) {}

CompactState::chunk_t CompactState::point(int pos) const {
    return board[pos / P_CHUNK] >> pos % P_CHUNK * B_POINT;
}

void CompactState::point(int pos, chunk_t mask) {
    // clang-format off
    
    board[pos / P_CHUNK] = board[pos / P_CHUNK]
        | mask << pos % P_CHUNK * B_POINT;

    // clang-format on
}

void CompactState::point(int pos, chunk_t val, chunk_t mask) {
    // clang-format off

    board[pos / P_CHUNK] = board[pos / P_CHUNK]
        & ~(mask << pos % P_CHUNK * B_POINT)
        | val << pos % P_CHUNK * B_POINT;

    // clang-format on
}

bool CompactState::canMove(chunk_t p) const { return (p & MOVABLE_MASK) == 0; }

void CompactState::firstApple() {}

void CompactState::moveTail() {}

} // namespace Snake