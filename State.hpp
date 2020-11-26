#ifndef COMPACT_STATE_HPP
#define COMPACT_STATE_HPP

#ifndef SNAKE_STATE_BOARD_WIDTH
#define SNAKE_STATE_BOARD_WIDTH 4
#define SNAKE_STATE_BOARD_HEIGHT 4
#endif

#include <array>

namespace Snake {

class State {
public:
    using chunk_type = uint_fast8_t;

    static constexpr int WIDTH = SNAKE_STATE_BOARD_WIDTH;
    static constexpr int HEIGHT = SNAKE_STATE_BOARD_HEIGHT;
    static constexpr int SIZE = WIDTH * HEIGHT;
    static constexpr int SPACE = SIZE * SIZE;

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

    // next apple constructor
    State(const State &prev);

    // step constructor
    State(const State &prev, chunk_type dir);

    bool canExplore(chunk_type dir) const;

    // ignores visited
    bool canMove(chunk_type dir) const;

    // value at point
    chunk_type value(int pos) const;

    chunk_type lastMove() const;

    bool operator==(const State &other) const;

    State &operator=(const State &other) = default;

private:
    static constexpr int B_POINT = 4; // bits per point
    static constexpr int P_CHUNK =    // point s per chunk
        sizeof(chunk_type) * 8 / B_POINT;

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

State::State() : head{}, tail{}, apple{}, length{(HEIGHT + 1) / 2}, board{} {
    for (int pos = 0; pos < BOARD_SIZE; ++pos) {
        board[pos] = 0U;
    }

    int x = (WIDTH + 1) / 2 - 1;

    head = x * HEIGHT + length - 1;
    tail = x * HEIGHT;

    for (int pos = tail + 1; pos <= head; ++pos) {
        point(pos, UP | OCCUPIED_MASK);
    }

    point(head, VISITED_MASK);
    point(tail, UP);

    eatApple();
}

State::State(const State &prev) :
        head{prev.head},
        tail{prev.tail},
        apple{prev.apple + 1},
        length{prev.length},
        board{prev.board} {
    while (apple != tail && (point(apple) & OCCUPIED_MASK) != 0U) {
        ++apple;
    }
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

State::State(const State &prev, chunk_type dir) :
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

    // set last move direction
    point(head, DIRECTION_MASK, dir);

    if (head == apple) {
        ++length;
        if (length != SIZE)
            eatApple();
    } else {
        moveTail();
    }
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

State::chunk_type State::lastMove() const {
    return point(head) & DIRECTION_MASK;
}

bool State::operator==(const State &other) const {
    if(head != other.head ||
        tail != other.tail ||
        apple != other.apple ||
        length != other.length)
        return false;
    for(int i=0; i<BOARD_SIZE; ++i)
        if(board[i] != other.board[i])
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