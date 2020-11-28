#ifndef COMPACT_STATE_HPP
#define COMPACT_STATE_HPP

#if !defined(SNAKE_STATE_BOARD_WIDTH) || !defined(SNAKE_STATE_BOARD_HEIGHT) || \
    !defined(SNAKE_STATE_START_LENGTH)
#define SNAKE_STATE_BOARD_WIDTH 4
#define SNAKE_STATE_BOARD_HEIGHT 4
#define SNAKE_STATE_START_LENGTH 2
#endif

#include <array>

namespace Snake {

class State {
public:
    using chunk_type = uint32_t;

    static constexpr int WIDTH = SNAKE_STATE_BOARD_WIDTH;
    static constexpr int HEIGHT = SNAKE_STATE_BOARD_HEIGHT;
    static constexpr int START_LENGTH = SNAKE_STATE_START_LENGTH;

    static_assert(START_LENGTH <= HEIGHT + 1 / 2, "start length too long");

    static constexpr int SIZE = WIDTH * HEIGHT;
    static constexpr int SPACE = SIZE * SIZE;
    static constexpr int DEPTH = SIZE - START_LENGTH;

    static constexpr chunk_type RIGHT = 0U;
    static constexpr chunk_type UP = 1U;
    static constexpr chunk_type LEFT = 2U;
    static constexpr chunk_type DOWN = 3U;
    static constexpr chunk_type HEAD = 4U;
    static constexpr chunk_type APPLE = 5U;
    static constexpr chunk_type EMPTY = 6U;
    static constexpr chunk_type VISITED = 7U;

    int head, tail, apple, length;

    // new game constructor
    State();

    // exclude occupied, visited
    bool canExplore(chunk_type dir) const;

    // exclude occupied, allow visited
    bool canMove(chunk_type dir) const;

    static State move(const State &prev, chunk_type dir);

    static State nextApple(const State &prev);

    // sets body points visited
    static State safety(const State &prev);

    // value at point
    chunk_type value(int pos) const;

    bool operator==(const State &other) const;

    State(const State &other) = default;

    State &operator=(const State &other) = default;

private:
    static constexpr int B_POINT = 4; // bits per point
    static constexpr int P_CHUNK = 1; // points per chunk

    static_assert(B_POINT * P_CHUNK <= 8 * sizeof(chunk_type),
                  "chunk does not fit in chunk_type");

    static constexpr int BOARD_SIZE = (SIZE - 1) / P_CHUNK + 1;

    std::array<chunk_type, BOARD_SIZE> board;

    chunk_type point(int pos) const;

    // set masked bits to true
    void point(int pos, chunk_type mask);

    // set masked bits to val
    void point(int pos, chunk_type mask, chunk_type val);

    static int step(int pos, chunk_type dir);

    void eatApple();

    void moveTail();

    // bit 0: direction axis
    // bit 1: direction sign
    // bit 2: visited
    // bit 3: occupied
    static constexpr chunk_type DIRECTION_MASK = 0b0011;
    static constexpr chunk_type EXPLORE_MASK = 0b1100;
    static constexpr chunk_type OCCUPIED_MASK = 0b0100;
    static constexpr chunk_type VISITED_MASK = 0b1000;
};

State::State() : head{}, tail{}, apple{}, length{START_LENGTH}, board{} {
    for (int pos = 0; pos < BOARD_SIZE; ++pos) {
        board[pos] = 0U;
    }

    int x = (WIDTH + 1) / 2 - 1;

    head = x * HEIGHT + length - 1;
    tail = x * HEIGHT;

    for (int pos = tail + 1; pos < head; ++pos) {
        point(pos, UP | OCCUPIED_MASK);
    }

    point(head, VISITED_MASK | OCCUPIED_MASK);
    point(tail, UP);

    eatApple();
}

bool State::canExplore(chunk_type dir) const {
    // clang-format off
    switch (dir) {
    case RIGHT:
        return head + HEIGHT < SIZE &&
               (point(step(head, dir)) & EXPLORE_MASK) == 0U;
    case UP:
        return head % HEIGHT < HEIGHT - 1 &&
               (point(step(head, dir)) & EXPLORE_MASK) == 0U;
    case LEFT:
        return head >= HEIGHT &&
                (point(step(head, dir)) & EXPLORE_MASK) == 0U;
    default:
        return head % HEIGHT != 0 &&
               (point(step(head, dir)) & EXPLORE_MASK) == 0U;
    }
    // clang-format on
}

bool State::canMove(chunk_type dir) const {
    // clang-format off
    switch (dir) {
    case RIGHT:
        return head + HEIGHT < SIZE &&
               (point(step(head, dir)) & OCCUPIED_MASK) == 0U;
    case UP:
        return head % HEIGHT < HEIGHT - 1 &&
               (point(step(head, dir)) & OCCUPIED_MASK) == 0U;
    case LEFT:
        return head >= HEIGHT &&
                (point(step(head, dir)) & OCCUPIED_MASK) == 0U;
    default:
        return head % HEIGHT != 0 &&
               (point(step(head, dir)) & OCCUPIED_MASK) == 0U;
    }
    // clang-format on
}

State State::move(const State &prev, chunk_type dir) {
    State next{prev};
    // set head direction
    next.point(next.head, DIRECTION_MASK, dir);

    // update head
    next.head = step(next.head, dir);

    // set head occupied and visited
    next.point(next.head, OCCUPIED_MASK | VISITED_MASK);

    if (next.head == next.apple) {
        ++next.length;
        if (next.length != SIZE) {
        }
        next.eatApple();
    } else {
        next.moveTail();
    }

    return next;
}

State State::nextApple(const State &prev) {
    State next{prev};

    ++next.apple;
    while (next.apple == next.tail ||
           (next.point(next.apple) & OCCUPIED_MASK) != 0U) {
        next.apple = (next.apple + 1) % SIZE;
    }

    return next;
}

State State::safety(const State &prev) {
    State next{prev};

    for (int pos = 0; pos < SIZE; ++pos) {
        if ((next.point(pos) & OCCUPIED_MASK) != 0U) {
            next.point(pos, VISITED_MASK);
        }
    }

    return next;
}

State::chunk_type State::value(int pos) const {
    if (pos == head)
        return HEAD;
    if (pos == apple)
        return APPLE;
    if (pos != tail && (point(pos) & OCCUPIED_MASK) == 0U)
        return EMPTY;
    return point(pos) & DIRECTION_MASK;
}

bool State::operator==(const State &other) const {
    if (head != other.head || tail != other.tail || apple != other.apple ||
        length != other.length)
        return false;
    for (int i = 0; i < BOARD_SIZE; ++i)
        if (board[i] != other.board[i])
            return false;
    return true;
}

State::chunk_type State::point(int pos) const {
    return board[pos / P_CHUNK] >> pos % P_CHUNK * B_POINT;
}

void State::point(int pos, chunk_type mask) {
    // clang-format off
    
    board[pos / P_CHUNK] = board[pos / P_CHUNK]
        | mask << pos % P_CHUNK * B_POINT;

    // clang-format on
}

void State::point(int pos, chunk_type mask, chunk_type val) {
    // clang-format off

    board[pos / P_CHUNK] = (board[pos / P_CHUNK]
        & ~(mask << pos % P_CHUNK * B_POINT))
        | (val << pos % P_CHUNK * B_POINT);

    // clang-format on
}

int State::step(int pos, chunk_type dir) {
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

void State::eatApple() {
    // reset visited
    for (int pos = 0; pos < SIZE; ++pos) {
        if (pos != head) {
            point(pos, VISITED_MASK, 0U);
        }
    }

    apple = 0;
    while (apple == tail || (point(apple) & OCCUPIED_MASK) != 0U) {
        ++apple;
    }
}

void State::moveTail() {
    tail = step(tail, point(tail) & DIRECTION_MASK);
    point(tail, OCCUPIED_MASK, 0U);
}

} // namespace Snake

#endif