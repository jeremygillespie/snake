# Snake
Snake is a game where you play as a mother duck in a rectangular grassy field of size *N* with fences on all sides. You have exactly *N* - 1 ducklings; some of them are following you in a line and the rest are hiding somewhere in the field. One at a time, the ducklings come out of hiding and wait for you to come find them. Once you reach a duckling, it gets in line behind you. You can't stop moving or the ducklings in line will bump into each other, and you can't cross the line of ducklings or they will lose their place in line. You win when you have found all of your ducklings. I don't know why it's called "snake".

## Strategy:
A vertex is a point that the snake can occupy. A corner is any set of four vertices adjacent vertices (a 2x2 square), or two vertices on the exterior of the graph. A corner starts unassigned and must be assigned L or R as long as one of its vertices is occupied.

    function move (direction dir)
      direction oldDir = lastmove.dir
      vertex Vstart = lastMove.Vend
      Cleft = Vstart + 0.5 * oldDir + 0.5 * turnLeft(oldDir)
      Cright = Vstart + 0.5 * oldDir + 0.5 * turnRight(oldDir)

      IF Cleft(Vend) AND Cright(Vend) THEN Cleft = L, Cright = R

      ELSE IF Cleft(Vend) THEN Cleft = R, Cright = R

      ELSE Cleft = L, Cright = L

This technique, in combination with a running total of moves(N, S, E, W) and turns(L, R) shall fulfill three tasks: maintain a cyclical path, detect loops/spirals/inaccessible vertices, and converge to a Hamiltonian path.

## To Do
- [ ] wall creation
- [x] A* search
- [x] decouple moves from framerate
- [ ] manhattan with reversals
- [ ] file saving/loading
- [ ] better argument parsing
- [ ] adjust A* space > time