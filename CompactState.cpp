#include "CompactState.hpp"

namespace Snake {

CompactState::CompactState() :
        head{},
        tail{},
        apple{},
        length{(HEIGHT + 1) / 2},
        board{} {
    for (size_type pos = 0; pos < board.size(); ++pos) {
        board[pos] = 0;
    }

    int x = (WIDTH + 1) / 2 - 1;

    head = x * HEIGHT + length - 1;
    tail = x * HEIGHT;

    for (size_type pos = tail + 1; pos <= head; ++pos) {
        point(pos, UP | OCCUPIED_MASK);
    }

    point(head, VISITED_MASK);
    point(tail, UP);

    eatApple();
}

CompactState::CompactState(const CompactState &prev) :
        head{prev.head},
        tail{prev.tail},
        apple{prev.apple + 1},
        length{prev.length},
        board{prev.board} {
    while (apple != tail && (point(apple) & OCCUPIED_MASK) != 0) {
        ++apple;
    }
}

bool CompactState::canExplore(chunk_type dir) const {
    // clang-format off
    switch (dir) {
    case RIGHT:
        return head + HEIGHT < SIZE &&
               (point(step(head, dir)) & EXPLORE_MASK) == 0;
    case UP:
        return head % HEIGHT < HEIGHT - 1 &&
               (point(step(head, dir)) & EXPLORE_MASK) == 0;
    case LEFT:
        return head >= HEIGHT &&
                (point(step(head, dir)) & EXPLORE_MASK) == 0;
    default:
        return head % HEIGHT != 0 &&
               (point(step(head, dir)) & EXPLORE_MASK) == 0;
    }
    // clang-format on
}

bool CompactState::canMove(chunk_type dir) const {
    // clang-format off
    switch (dir) {
    case RIGHT:
        return head + HEIGHT < SIZE &&
               (point(step(head, dir)) & OCCUPIED_MASK) == 0;
    case UP:
        return head % HEIGHT < HEIGHT - 1 &&
               (point(step(head, dir)) & OCCUPIED_MASK) == 0;
    case LEFT:
        return head >= HEIGHT &&
                (point(step(head, dir)) & OCCUPIED_MASK) == 0;
    default:
        return head % HEIGHT != 0 &&
               (point(step(head, dir)) & OCCUPIED_MASK) == 0;
    }
    // clang-format on
}

CompactState::CompactState(const CompactState &prev, chunk_type dir) :
        head{prev.head},
        tail{prev.tail},
        apple{prev.apple},
        length{prev.length},
        board{prev.board} {
    // set head direction
    point(head, DIRECTION_MASK, dir);

    // update head
    head = step(head, dir);

    // set head occupied and visited
    point(head, OCCUPIED_MASK | VISITED_MASK);

    if (head == apple) {
        ++length;
        if (length != SIZE)
            eatApple();
    } else {
        moveTail();
    }
}

CompactState::chunk_type CompactState::value(size_type pos) const {
    if (pos == head)
        return HEAD;
    if (pos == apple)
        return APPLE;
    if (pos != tail && (point(pos) & OCCUPIED_MASK) == 0)
        return EMPTY;
    return point(pos) & DIRECTION_MASK;
}

CompactState::chunk_type CompactState::point(size_type pos) const {
    return board[pos / P_CHUNK] >> pos % P_CHUNK * B_POINT;
}

void CompactState::point(size_type pos, chunk_type mask) {
    // clang-format off
    
    board[pos / P_CHUNK] = board[pos / P_CHUNK]
        | mask << pos % P_CHUNK * B_POINT;

    // clang-format on
}

void CompactState::point(size_type pos, chunk_type mask, chunk_type val) {
    // clang-format off

    board[pos / P_CHUNK] = (board[pos / P_CHUNK]
        & ~(mask << pos % P_CHUNK * B_POINT))
        | (val << pos % P_CHUNK * B_POINT);

    // clang-format on
}

CompactState::size_type CompactState::step(size_type pos, chunk_type dir) {
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

void CompactState::eatApple() {
    // reset visited
    for (size_type pos = 0; pos < SIZE; ++pos) {
        if (pos != head) {
            point(pos, VISITED_MASK, 0);
        }
    }

    apple = 0;
    while (apple != tail && (point(apple) & OCCUPIED_MASK) != 0) {
        ++apple;
    }
}

void CompactState::moveTail() {
    tail = step(tail, point(tail) & DIRECTION_MASK);
    point(tail, OCCUPIED_MASK, 0);
}

} // namespace Snake