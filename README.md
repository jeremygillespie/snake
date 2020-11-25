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

## To Do
* <s>create state</s>
* <s>add DFS</s>
* add RBFS
* create benchmarks
* add pathfinding
* add monticarlo
* <s>optimize state</s>
* optimize RBFS
* optimize pathfinding
* add graphics
* add stats