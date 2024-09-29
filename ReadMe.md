# Labyrinth
After Galgenmann has make fun to make, next game should be a simple Labyrinth. Still pur C++, running in the terminal.

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

### Event at specific position
* (/) reach the "Goal-Zone" to win the game

### Collisionrecognition with Wall
* (/) draw "Walls" as obstacles
* maybe we need here a algorithm to generate a solveable labyrinth --> maybe later, it's complicated

## Styleguide
a little Styleguide to get a constant naming
* CLASS: full capitalized
* FunctionsAndMethods: camelCase
* variables_and_properties: snake_case
* Constants: full capitalized