* <s>create state</s>
* <s>add minmax</s>
* add alpha-beta
* add pathfinding heuristics
* optimize state
  * uint[w] curr. body position
  * uint[w] prev. head positions
  * uint[2*(n/32+1)] body directions
* optimize space search
 * is it acceptable to sum the chance nodes instead of average?
  * worst case becomes 1 + n - tSA n*(sum(1..n-len))
* optimize heuristics
 * cost of death = 1 + worst case success = 1 + n - tSA + n*(n-len)
* add graphics
* add stats
* add monticarlo
