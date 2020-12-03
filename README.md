# Snake
Snake is a game where you play as a mother duck in a rectangular grassy field of size *N* with fences on all sides. You have exactly *N* - 1 ducklings; some of them are following you in a line and the rest are hiding somewhere in the field. One at a time, the ducklings come out of hiding and wait for you to come find them. Once you reach a duckling, it gets in line behind you. You can't stop moving or the ducklings in line will bump into each other, and you can't cross the line of ducklings or they will lose their place in line. You win when you have found all of your ducklings. I don't know why it's called "snake".

## Strategy:
A vertex is a point that the snake can occupy. An edge connects two vertices. A corner is a set of four adjacent vertices and edges, or two vertices and one edge on the exterior of the graph. A corner starts unassigned and can be marked L or R. Each edge has one corner to its left and one corner to its right.

    function move (direction dir)
      direction oldDir = lastmove.dir
      vertex Vstart = lastMove.Vend
      CL = Vstart + 0.5 * oldDir + 0.5 * turnLeft(oldDir)
      CR = Vstart + 0.5 * oldDir + 0.5 * turnRight(oldDir)

      IF cL(Vend) AND cR(Vend) THEN cL = L, cR = R

      ELSE IF cL(Vend) THEN cL = R, cR = R

      ELSE cL = L, cR = L

If a move would mark a corner the opposite of what it is already marked, you cannot make that move.

If a section of the graph is surrounded by corners that are all marked the same, then that section is inaccessible until the snake passes.

## To Do
- [x] variable size state
- [ ] playable graphics
- [ ] search