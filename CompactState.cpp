#include "CompactState.hpp"

namespace Snake {

CompactState::CompactState() :
        head{},
        tail{},
        apple{},
        length{(HEIGHT + 1) / 2},
        board{} {
    for (int i = 0; i < board.size(); ++i) {
        board[i] = 0;
    }

    int x = (WIDTH + 1) / 2 - 1;

    head = x * HEIGHT + length - 1;
    tail = x * HEIGHT;

    for (int pos = tail + 1; pos <= head; ++pos) {
        point(pos, UP & OCCUPIED_MASK);
    }

    point(head, VISITED_MASK);

    eatApple();
}

CompactState::CompactState(const CompactState &prev) :
        head{prev.head},
        tail{prev.tail},
        apple{prev.apple + 1},
        length{prev.length},
        board{prev.board} {
    while (apple != tail && point(apple) & OCCUPIED_MASK != 0) {
        ++apple;
    }
}

bool CompactState::canMove(chunk_t dir) const {
    switch (dir) {
    case RIGHT:
        return head + HEIGHT < SIZE && movable(point(step(head, dir)));
    case UP:
        return head % HEIGHT < HEIGHT - 1 && movable(point(step(head, dir)));
    case LEFT:
        return head - HEIGHT >= 0 && movable(point(step(head, dir)));
    default:
        return head % HEIGHT > 0 && movable(point(step(head, dir)));
    }
}

CompactState::CompactState(const CompactState &prev, chunk_t dir) {
    // set head direction
    point(head, DIRECTION_MASK, dir);

    // update head
    head = step(head, dir);

    // set head occupied and visited
    point(head, OCCUPIED_MASK & VISITED_MASK);

    if (head == apple) {
        eatApple();
    } else {
        moveTail();
    }
}

CompactState::chunk_t CompactState::point(int pos) const {
    return board[pos / P_CHUNK] >> pos % P_CHUNK * B_POINT;
}

void CompactState::point(int pos, chunk_t mask) {
    // clang-format off
    
    board[pos / P_CHUNK] = board[pos / P_CHUNK]
        | mask << pos % P_CHUNK * B_POINT;

    // clang-format on
}

void CompactState::point(int pos, chunk_t mask, chunk_t val) {
    // clang-format off

    board[pos / P_CHUNK] = board[pos / P_CHUNK]
        & ~(mask << pos % P_CHUNK * B_POINT)
        | val << pos % P_CHUNK * B_POINT;

    // clang-format on
}

int CompactState::step(int pos, chunk_t dir) {
    switch (dir) {
    case RIGHT:
        return pos + HEIGHT;
    case UP:
        return pos + 1;
    case LEFT:
        return pos - HEIGHT;
    default:
        return pos - 1;
    }
}

bool CompactState::movable(chunk_t p) const { return (p & MOVABLE_MASK) == 0; }

void CompactState::eatApple() {
    ++length;

    // reset visited
    for (int pos = 0; pos < SIZE; ++pos) {
        if (pos != head) {
            point(pos, VISITED_MASK, 0);
        }
    }

    apple = 0;
    while (apple != tail && point(apple) & OCCUPIED_MASK != 0) {
        ++apple;
    }
}

void CompactState::moveTail() {
    tail = step(tail, point(tail) & DIRECTION_MASK);
    point(tail, OCCUPIED_MASK, 0);
}

} // namespace Snake