# Labyrinth
After Galgenmann has make fun to make, next ste

## Compile
I use simple g++ for compiling
> g++ -std=c++17 -o Laybrinth Laybrinth.cpp

## Start the game
Use this in the folder containing the Galgenmann-Binary (tests on a Mac only)
> ./Laybrinth
**words.txt has to be next to the binary!**

## Steps
### Clear Screen
* (/) Takeover from Galgenmann

### Draw Ball to a Cursorpositopn
* (/) given as Row-Col-Vector

### Move the Ball
* (/) trough Arrow keys

### Adjust Temrinal Size
* Terminalwindow
* Fontsize

### Event at specific position
* reach the "Goal-Zone" to win the game

### Collisionrecognition with Wall
* draw "Walls" as obstacles
* maybe we need here a algorithm to generate a solveable labyrinth