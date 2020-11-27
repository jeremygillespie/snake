# Snake
Snake is a game where you play as a mother duck in a rectangular grassy field of size *N* with fences on all sides. You have exactly *N* - 1 ducklings; some of them are following you in a line and the rest are hiding somewhere in the field. One at a time, the ducklings come out of hiding and wait for you to come find them. Once you reach a duckling, it gets in line behind you. You can't stop moving or the ducklings in line will bump into each other, and you can't cross the line of ducklings or they will lose their place in line. You win when you have found all of your ducklings. I don't know why it's called "snake".

## Safety check
A key part of completeness is the safety check. Can the snake reach its own tail? Requires counting duration of each body segment. Can use visited mask as target.

## Math
width *W* * height *H* = size *N*

snake length *L*

apples left *A* = *N* - *L*

initial length *Li*

time since start of run *T*

time since last apple *Ta*

longest path to next apple without loops = *N* - *Ta* - 1

longest possible run = *T* + (*N* - 1) * *A* + *N* - *Ta* - 1

cost of success = *T*

cost of failure = longest possible run + 1

## Measuring Performance
Completeness: Success rate. min = 0%, max = optimal

[Cost] Optimality: Steps taken. min = Hamiltonian path avg, max = optimal

[Time] Complexity: Computation time. min = 1s, max = DFS (log scale)

## To Do
* <s>create state</s>
* <s>add DFS</s>
* <s>create compact state</s>
* create RBFS engine
  * <s>Path</s>
  * <s>DFS</s>
  * Safety check
* create fast state
* add stats
* compare state types

## Coming Soon
* graphics
* Monticarlo search
* pathfinding
* benchmarks