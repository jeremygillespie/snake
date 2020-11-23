#ifndef STATE_HPP
#define STATE_HPP

#include <vector>

using std::vector;

namespace Snake {

enum class direction { right, left, up, down };

class State {
public:
    static constexpr int APPLE = -1;
    static constexpr int EMPTY = 0;
    static constexpr int WIDTH = 6;
    static constexpr int HEIGHT = 6;
    static constexpr int SIZE = WIDTH * HEIGHT;

    // new state
    State();

    // state after move
    State(const State &prev, direction dir);

    // state with next possible apple
    // equivalent to n=1
    State(const State &prev);

    // state with apple offset by n spaces
    State(const State &prev, int n);

    int head, apple, length, time, timeSinceApple;

    bool canMove(direction dir) const;

    int &val(int p);
    int val(int p) const;

    int point(int p, direction dir) const;
    int point(direction dir) const;
    int point(int x, int y) const;

    int xCoord(int p) const;
    int yCoord(int p) const;

    State &operator=(const State &other) = default;

private:
    vector<int> board;
    vector<bool> visited;

    void nextApple(int p);
};

} // namespace Snake

#endif