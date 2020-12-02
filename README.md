# Snake
Snake is a game where you play as a mother duck in a rectangular grassy field of size *N* with fences on all sides. You have exactly *N* - 1 ducklings; some of them are following you in a line and the rest are hiding somewhere in the field. One at a time, the ducklings come out of hiding and wait for you to come find them. Once you reach a duckling, it gets in line behind you. You can't stop moving or the ducklings in line will bump into each other, and you can't cross the line of ducklings or they will lose their place in line. You win when you have found all of your ducklings. I don't know why it's called "snake".

## Strategy
Each space is diagonally adjacent to two controllers. When you enter a space, the controller on the side from which you entered assigns its polarity to match the direction from which you entered. When you leave a space, the other controller assigns its polarity to match the direciton in which you left, regardless of its adjacency. You may not enter a space if your direction has the opposite polarity of the adjacent controller. If any part of the snake is touching the edge, all edge segments have the polarity of that part. If you entered from the positive direction and your negative exit is already marked, you must take the negative exit.

## To Do
- [ ] variable size state
- [ ] playable graphics
- [ ] search