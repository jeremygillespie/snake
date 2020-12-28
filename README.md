# Snake
Snake is a game where you play as a mother duck in a rectangular grassy field of size *N* with fences on all sides. You have exactly *N* - 1 ducklings; some of them are following you in a line and the rest are hiding somewhere in the field. One at a time, the ducklings come out of hiding and wait for you to come find them. Once you reach a duckling, it gets in line behind you. You can't stop moving or the ducklings in line will bump into each other, and you can't cross the line of ducklings or they will lose their place in line. You win when you have found all of your ducklings. I don't know why it's called "snake".

## Strategy:
Counting corners guarantees safety. Time to maximize A*.

## To Do

- [x] A* search
- [x] stats
- [x] show search path
- [x] corner safety check
- [ ] A* with reversals
- [ ] improve search algorithm
- [ ] visual stats
- [ ] wall creation
- [ ] file saving/loading