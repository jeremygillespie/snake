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
    static constexpr int WIDTH = SNAKE_STATE_BOARD_WIDTH;
    static constexpr int HEIGHT = SNAKE_STATE_BOARD_HEIGHT;
    static constexpr int START_LENGTH = SNAKE_STATE_START_LENGTH;

    static_assert(START_LENGTH <= HEIGHT + 1 / 2, "start length too long");

    static constexpr int SIZE = WIDTH * HEIGHT;
    static constexpr int SPACE = SIZE * SIZE;
    static constexpr int DEPTH = SIZE - START_LENGTH;

    static constexpr unsigned char RIGHT = 0U, UP = 1U, LEFT = 2U, DOWN = 3U,
                                   HEAD = 4U, APPLE = 5U, EMPTY = 6U,
                                   VISITED = 7U;

    int head, tail, apple, length;

    // new game constructor
    State();

    // exclude occupied, allow visited
    bool canMove(unsigned char dir) const;

    // exclude occupied, exclude visited
    bool canExplore(unsigned char dir) const;

    // exclude occupied, require visited
    bool canLoop(unsigned char dir) const;

    // successor state after move
    static State move(const State &prev, unsigned char dir);

    // incrememnt apple location
    static State nextApple(const State &prev);

    // sets body vertices visited
    static State safety(const State &prev);

    // value at vertex
    unsigned char value(int pos) const;

    bool operator==(const State &other) const;

    State(const State &other) = default;

    State &operator=(const State &other) = default;

private:
    static constexpr int VERTICES_SIZE = (SIZE - 1) + 1;

    std::array<unsigned char, VERTICES_SIZE> vertices;

    unsigned char vertex(int pos) const;

    // set masked bits to true
    void vertex(int pos, unsigned char mask);

    // set masked bits to val
    void vertex(int pos, unsigned char mask, unsigned char val);

    static int step(int pos, unsigned char dir);

    // bit 0: direction axis
    // bit 1: direction sign
    // bit 2: visited
    // bit 3: occupied
    static constexpr unsigned char DIRECTION_MASK = 0b0011,
                                   EXPLORE_MASK = 0b1100,
                                   OCCUPIED_MASK = 0b0100,
                                   VISITED_MASK = 0b1000;
};

State::State() : head{}, tail{}, apple{}, length{START_LENGTH}, vertices{} {
    for (int pos = 0; pos < VERTICES_SIZE; ++pos) {
        vertices[pos] = 0U;
    }

    int x = (WIDTH + 1) / 2 - 1;

    head = x * HEIGHT + length - 1;
    tail = x * HEIGHT;

    for (int pos = tail + 1; pos < head; ++pos) {
        vertex(pos, UP | OCCUPIED_MASK);
    }

    vertex(head, VISITED_MASK | OCCUPIED_MASK);
    vertex(tail, UP);

    apple = head;
}

bool State::canMove(unsigned char dir) const {
    // clang-format off
    switch (dir) {
    case RIGHT:
        return head + HEIGHT < SIZE &&
               (vertex(step(head, dir)) & OCCUPIED_MASK) == 0U;
    case UP:
        return head % HEIGHT < HEIGHT - 1 &&
               (vertex(step(head, dir)) & OCCUPIED_MASK) == 0U;
    case LEFT:
        return head >= HEIGHT &&
                (vertex(step(head, dir)) & OCCUPIED_MASK) == 0U;
    default:
        return head % HEIGHT != 0 &&
               (vertex(step(head, dir)) & OCCUPIED_MASK) == 0U;
    }
    // clang-format on
}

bool State::canExplore(unsigned char dir) const {
    // clang-format off
    switch (dir) {
    case RIGHT:
        return head + HEIGHT < SIZE &&
               (vertex(step(head, dir)) & EXPLORE_MASK) == 0U;
    case UP:
        return head % HEIGHT < HEIGHT - 1 &&
               (vertex(step(head, dir)) & EXPLORE_MASK) == 0U;
    case LEFT:
        return head >= HEIGHT &&
                (vertex(step(head, dir)) & EXPLORE_MASK) == 0U;
    default:
        return head % HEIGHT != 0 &&
               (vertex(step(head, dir)) & EXPLORE_MASK) == 0U;
    }
    // clang-format on
}

bool State::canLoop(unsigned char dir) const {
    // clang-format off
    switch (dir) {
    case RIGHT:
        return head + HEIGHT < SIZE &&
               (vertex(step(head, dir)) & EXPLORE_MASK) == VISITED_MASK;
    case UP:
        return head % HEIGHT < HEIGHT - 1 &&
               (vertex(step(head, dir)) & EXPLORE_MASK) == VISITED_MASK;
    case LEFT:
        return head >= HEIGHT &&
                (vertex(step(head, dir)) & EXPLORE_MASK) == VISITED_MASK;
    default:
        return head % HEIGHT != 0 &&
               (vertex(step(head, dir)) & EXPLORE_MASK) == VISITED_MASK;
    }
    // clang-format on
}

State State::move(const State &prev, unsigned char dir) {
    State next{prev};
    // set head direction
    next.vertex(next.head, DIRECTION_MASK, dir);

    // update head
    next.head = step(next.head, dir);

    // set head occupied and visited
    next.vertex(next.head, OCCUPIED_MASK | VISITED_MASK);

    // apple eaten
    if (next.head == next.apple) {
        ++next.length;
        if (next.length != SIZE) {
            // reset visited
            for (int pos = 0; pos < SIZE; ++pos) {
                if (pos != next.head) {
                    next.vertex(pos, VISITED_MASK, 0U);
                }
            }
        }

    } else {
        // move tail
        next.tail = step(next.tail, next.vertex(next.tail) & DIRECTION_MASK);
        next.vertex(next.tail, OCCUPIED_MASK, 0U);
    }

    return next;
}

State State::nextApple(const State &prev) {
    State next{prev};

    if (next.head == next.apple) {
        next.apple = 0;
    } else {
        ++next.apple;
    }

    while (next.apple == next.tail ||
           (next.vertex(next.apple) & OCCUPIED_MASK) != 0U) {
        next.apple = (next.apple + 1) % SIZE;
    }

    return next;
}

State State::safety(const State &prev) {
    State next{prev};

    for (int pos = 0; pos < SIZE; ++pos) {
        if ((next.vertex(pos) & OCCUPIED_MASK) != 0U) {
            next.vertex(pos, VISITED_MASK);
        }
    }

    return next;
}

unsigned char State::value(int pos) const {
    if (pos == head)
        return HEAD;
    if (pos == apple)
        return APPLE;
    if (pos != tail && (vertex(pos) & OCCUPIED_MASK) == 0U)
        return EMPTY;
    return vertex(pos) & DIRECTION_MASK;
}

bool State::operator==(const State &other) const {
    if (head != other.head || tail != other.tail || apple != other.apple ||
        length != other.length)
        return false;
    for (int i = 0; i < VERTICES_SIZE; ++i)
        if (vertices[i] != other.vertices[i])
            return false;
    return true;
}

unsigned char State::vertex(int pos) const { return vertices[pos]; }

void State::vertex(int pos, unsigned char mask) {
    vertices[pos] = vertices[pos] | mask;
}

void State::vertex(int pos, unsigned char mask, unsigned char val) {
    vertices[pos] = (vertices[pos] & ~mask) | val;
}

int State::step(int pos, unsigned char dir) {
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

} // namespace Snake

#endif