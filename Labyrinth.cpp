#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>

class Labyrinth{
/* our class for the Labyrinth game */
private:
    std::vector<int> position;

public: 
    void clear_screen() {
        /* clear the terminal, so that only the game in the current round is visible */
        std::cout << "\033[2J\033[H"; // Clear Screen and position the Cursor
        std::cout << "\033[?25l"; // Hide the Cursor
    }

    void set_position(int row, int col) {
        /* set the cursorposition */
        position = {row, col};
    }

    void set_Raw() {
        /* activate RawMode, so that we haven't to wait for a carriage return after input */
        struct termios terminal;

        tcgetattr(STDIN_FILENO, &terminal);

        terminal.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
    }

    void move_position(int row, int col) {
        /* move position of the cursor */
        position[0] += row;
        position[1] += col;
        if(position[0] < 1) position[0] = 1; // to stay in screen
        if(position[1] < 1) position[1] = 1; // to stay in screen
    }

    void set_cursor() {
        /* set cursor to the current position */ 
        std::cout << "\033[" << position[0] << ";" << position[1] << "H";
    }

    void drawBall() const {
        /* draw the ball on the current cursor position */
        std::cout << "○";
    }

    void moveCursor() {
        /* move the cursor*/
        char input;
        input = getchar();
        if(input == '\033') {
            getchar(); // skip ']' 
            switch(getchar()) {
                case 'A': // up
                    move_position(-1, 0);
                    break;
                case 'B': // down
                    move_position(1, 0);
                    break;
                case 'C': // right
                    move_position(0, 1);
                    break;
                case 'D': // left
                    move_position(0, -1);
                    break;
            }
        }
    }

    bool ask_replay() {
        /* ask for replay */
        char userIn;
        std::cout << "Neues Spiel? (j):  ";
        std::cin >> userIn;
        return(userIn == 'j');
    }
};

void initialize_game(Labyrinth &labyrinth) {
    /* start the game at position 30,30, activate row-mode*/
    labyrinth.set_Raw();
    labyrinth.set_position(30, 30);
    labyrinth.set_cursor();
};

void play_game(Labyrinth &labyrinth) {
    /* we have no exit-condition yet, so let play us 'rounds' rounds*/
    int rounds = 100;
    for(int count = 0; count < rounds; count++) {
        labyrinth.clear_screen();
        labyrinth.set_cursor();
        labyrinth.drawBall();
        labyrinth.moveCursor();
    }
};

bool end_game(Labyrinth &labyrinth) {
    //Draw a last time
    labyrinth.clear_screen();
    labyrinth.set_cursor();
    labyrinth.drawBall();
    // ask for replay
    std::cout << "\n\n\n\n" << std::endl;
    bool replay = labyrinth.ask_replay();
    return(replay);
};

int main(){
    /* mainfunction, loop around the game for a replay*/
    bool replay = false;
    do {
        Labyrinth labyrinth;
        initialize_game(labyrinth);
        play_game(labyrinth);
        replay = end_game(labyrinth);
    } while(replay);
}