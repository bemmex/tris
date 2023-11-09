# tris
terminal tris game with ncurses, implementation in c for linux, using the mouse.

## game images

<img src="img/tris-1.png" alt="game" width="300px">
<img src="img/tris-2.png" alt="game" width="300px">
<img src="img/tris-3.png" alt="game" width="300px">

## controls

- Esc : Quit Game
- F3: Change Gamemode
    1. Two Player
    2. Player - PC
- F5: New Match
- Mouse click on the cell to play

## how to compile

1. make
2. make run

## Info
For the game mode (PLAYER-PC), for PC movements, i implemented a hardened random walk generator, 
which checks whether it can win or block the opponent's victory

### Dependencies
Before compiling make sure you have ncurses dev install
