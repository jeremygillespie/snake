#ifndef COMPACT_STATE_HPP
#define COMPACT_STATE_HPP

#ifndef SNAKE_STATE_BOARD_WIDTH
#define SNAKE_STATE_BOARD_WIDTH 4
#define SNAKE_STATE_BOARD_HEIGHT 4
#endif

#include <array>
#include <cstdint>

namespace Snake {

class CompactState {
public:
    using chunk_t = uint_fast8_t;

    static constexpr int WIDTH = SNAKE_STATE_BOARD_WIDTH;
    static constexpr int HEIGHT = SNAKE_STATE_BOARD_HEIGHT;
    static constexpr int SIZE = WIDTH * HEIGHT;
    static constexpr int SPACE = SIZE * SIZE;

    int head, tail, apple, length;

    // new game constructor
    CompactState();

    // next apple constructor
    CompactState(const CompactState &prev);

    bool canMove(chunk_t dir) const;

    // step constructor
    CompactState(const CompactState &prev, chunk_t dir);

private:
    static constexpr int B_POINT = 4; // bits per point
    static constexpr int P_CHUNK =
        sizeof(chunk_t) * 8 / B_POINT; // points per chunk

    std::array<chunk_t, (SIZE - 1) / P_CHUNK + 1> board;

    chunk_t point(int pos) const;

    // set masked bits to true
    void point(int pos, chunk_t mask);

    // set masked bits to val
    void point(int pos, chunk_t mask, chunk_t val);

    static int step(int pos, chunk_t dir);

    bool movable(chunk_t p) const;

    void eatApple();

    void moveTail();

    // bit 0: direction axis
    // bit 1: direction sign
    // bit 2: visited
    // bit 3: occupied
    static constexpr chunk_t RIGHT = 0b0000;
    static constexpr chunk_t UP = 0b0001;
    static constexpr chunk_t LEFT = 0b0010;
    static constexpr chunk_t DOWN = 0b0011;

    static constexpr chunk_t DIRECTION_MASK = 0b0011;
    static constexpr chunk_t MOVABLE_MASK = 0b1100;
    static constexpr chunk_t OCCUPIED_MASK = 0b0100;
    static constexpr chunk_t VISITED_MASK = 0b1000;
};

} // namespace Snake

#endif