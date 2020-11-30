# Snake
Snake is a game where you play as a mother duck in a rectangular grassy field of size *N* with fences on all sides. You have exactly *N* - 1 ducklings; some of them are following you in a line and the rest are hiding somewhere in the field. One at a time, the ducklings come out of hiding and wait for you to come find them. Once you reach a duckling, it gets in line behind you. You can't stop moving or the ducklings in line will bump into each other, and you can't cross the line of ducklings or they will lose their place in line. You win when you have found all of your ducklings. I don't know why it's called "snake".

## Safety check
A key part of completeness is the safety check. Can the snake reach its own tail? Requires counting duration of each body segment. Can use visited mask as target.

## Math
### Definitions
width * height = size *N*

longest path to apple without loops = *N* - 1

current head position *H*

a loop is when *H* = *Hp* for any *Hp* that *H* occupied since eating the last apple, including the position of the last apple.

safety point *S* = any point occupied since eating the last apple which is not currently occupied

#### Conjecture
It is never correct nor optimal perform a loop.

#### Conjecture
If *H* can reach *S* without eating an apple, then it is possible to win the game without dying


## Measuring Performance
Completeness: Success rate. min = 0%, max = optimal

[Cost] Optimality: Steps taken. min = Hamiltonian path avg, max = optimal

[Time] Complexity: Computation time. min = 1s, max = DFS (log scale)

## To Do
- [x] state
- [x] exhaustive engine
- [ ] stats
  - [ ] timer
  - [ ] progress bar
- [ ] heuristic engine
  - [ ] heuristic apple search
  - [ ] safety search

## Coming Soon
* graphics
* stats
* benchmarks
* better heuristics