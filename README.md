# Naive Branch
### This branch contains a verbose version of depth-first search.

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