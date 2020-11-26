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
* create fast state
* add stats
* compare state types

## Coming Soon
* graphics
* Monticarlo search
* pathfinding
* benchmarks