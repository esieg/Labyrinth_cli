#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <random>
#include <ctime>
#include <string>
#include <cstdlib> 
#include <stack>

class LABYRINTH{
/* our class for the Labyrinth game */
private:
    static const int SIZE = 40;
    static const int WALL = 1;
    static const int WALLS = 100;

    std::vector<std::vector<int>> walls;
    std::vector<int> ball;
    std::vector<int> goal;
    
public: 
    bool win = false;
    static const int ROUNDS = 100;

    void clearScreen() {
        /* clear the terminal, so that only the game in the current round is visible */
        std::cout << "\033[2J\033[H"; // Clear Screen and position the Cursor
        std::cout << "\033[?25l"; // Hide the Cursor
    }

    void generateWalls() {
        /* draw some obstacles */
        // first, resize walls and initialize with WAY
        walls.resize(SIZE, std::vector<int>(SIZE, 0));
        // second, set the border walls
        for (int i = 0; i < SIZE; ++i) {
            walls[0][i] = WALL; // Upper Wall
            walls[SIZE - 1][i] = WALL; // Lower Wall
            walls[i][0] = WALL; // Left Wall
            walls[i][SIZE - 1] = WALL; // Right Wall
        }
        // set WALLS walls
        std::mt19937 generator(static_cast<unsigned int>(std::time(0)));
        std::uniform_int_distribution<int> distribution(1,SIZE-1);
        for(int wall = 0; wall < WALLS; wall++) {
            walls[distribution(generator)][distribution(generator)] = WALL;
        }
    }

    void setBallGoalByRandom() {
        /* set goal to a random position (in x,y 2 to SIZE-2)*/
        std::mt19937 generator(static_cast<unsigned int>(std::time(0)));
        std::uniform_int_distribution<int> distribution(2, SIZE-2);
        // we need two coordinates, where no wall is
        int free_pos;
        std::vector<int> pos_goal;
        std::vector<int> pos_ball;
        do{
            free_pos = 0;
            pos_goal = {distribution(generator), distribution(generator)};
            pos_ball = {distribution(generator), distribution(generator)};
            if (walls[pos_goal[0]][pos_goal[1]] != WALL) {
                free_pos += 1;
            }
            if ((walls[pos_goal[0]][pos_goal[1]] != WALL) && (pos_goal != pos_ball)) {
                free_pos += 1;
            }
        } while(free_pos < 2);
        goal = pos_goal;
        ball = pos_ball;
    }

    void setRaw() {
        /* activate RawMode, so that we haven't to wait for a carriage return after input */
        struct termios terminal;

        tcgetattr(STDIN_FILENO, &terminal);

        terminal.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
    }

    void drawWalls() {
        /* draw the walls */
        for(int row = 0; row < SIZE; row++) {
            for(int col = 0; col < SIZE; col++) {
                if (walls[row][col] == WALL) {
                    std::cout << "\033[" << row+1 << ";" << col << "H";
                    std::cout << "# ";  // Wall
               }
            }
        }
    }

    void drawBall() const {
        /* draw the ball on his current position */
        std::cout << "\x1b[31m";
        std::cout << "\033[" << ball[0]+1 << ";" << ball[1] << "H";
        if(!win) {
            std::cout << "●";
        } else {
            std::cout << "⬤";
        } 
        std::cout << "\x1b[0m";
    }

    void drawGoal() const {
        /* draw the goal on his position */
        std::cout << "\x1b[31m";
        std::cout << "\033[" << goal[0]+1 << ";" << goal[1] << "H";
        if(!win) {
            std::cout << "○";
        }
        std::cout << "\x1b[0m";
    }

    void drawRounds(int count) const {
        /* draw the remaining rounds */
        std::cout << "\033[" << SIZE+1<< ";" << 0 << "H";
        std::cout << count << " von " << ROUNDS << std::endl;
    }

    void checkGoalFound() {
        /* check if goal is reached */
        if(ball[0] == goal[0] && ball[1] == goal[1]) {
            win = true;
        }
    }

    void moveBall() {
        /* move the ball */
        char input;
        input = getchar();
        int new_value;
        if(input == '\033') {
            getchar(); // skip ']' 
            switch(getchar()) {
                case 'A': // up
                    new_value = ball[0] - 1;
                    ball[0] = (walls[new_value][ball[1]] == WALL) ? ball[0] : new_value;
                    break;
                case 'B': // down
                    new_value = ball[0] + 1;
                    ball[0] = (walls[new_value][ball[1]] == WALL) ? ball[0] : new_value;
                    break;
                case 'C': // right
                    new_value = ball[1] + 1;
                    ball[1] = (walls[ball[0]][new_value] == WALL) ? ball[1] : new_value;
                    break;
                case 'D': // left
                    new_value = ball[1] - 1;
                    ball[1] = (walls[ball[0]][new_value] == WALL) ? ball[1] : new_value;
                    break;
            }
        }
    }

    void drawGameEnd() {
        /* show win/loose */
        std::cout << "\033[" << SIZE+1<< ";" << 0 << "H";
        std::string end_msg = win ? "Du hast gewonnen!" : "Leider verloren!";
        std::cout << end_msg << std::endl;
    }

    bool askReplay() {
        /* ask for replay */
        char user_in;
        std::cout << "Neues Spiel? (j):  ";
        std::cin >> user_in;
        return(user_in == 'j');
    }
};

void initializeGame(LABYRINTH &labyrinth) {
    /* initialie the game, activate row-mode*/
    labyrinth.setRaw();
    labyrinth.generateWalls();
    labyrinth.setBallGoalByRandom();
};

void playGame(LABYRINTH &labyrinth) {
    /* play max 100 rounds */
    for(int count = 0; count < labyrinth.ROUNDS; count++) {
        labyrinth.clearScreen();
        std::cout << "\n";
        labyrinth.drawWalls();
        labyrinth.drawBall();
        labyrinth.drawGoal();
        labyrinth.drawRounds(count);
        labyrinth.moveBall();
        labyrinth.checkGoalFound();
        if(labyrinth.win){
            break;
        }
    }
};

bool endGame(LABYRINTH &labyrinth) {
    /* end the game */
    // draw a last time
    labyrinth.clearScreen();
    labyrinth.drawWalls();
    labyrinth.drawBall();
    labyrinth.drawGoal();
    // end the game
    labyrinth.drawGameEnd();
    bool replay = labyrinth.askReplay();
    return(replay);
};

int main(){
    /* mainfunction, loop around the game for a replay*/
    bool replay = false;
    do {
        LABYRINTH labyrinth;
        initializeGame(labyrinth);
        playGame(labyrinth);
        replay = endGame(labyrinth);
    } while(replay);
}