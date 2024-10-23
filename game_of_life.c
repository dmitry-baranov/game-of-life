#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define LENGTH 80
#define HEIGHT 25

int gameOfLife(int height, int length);
char** makeBoard(int height, int length);
void initializeBoard(char** board, int height, int length);
void displayBoard(char** board, int height, int length);
void updateBoard(char** source, char** target, int height, int length);
int countNeighbors(char** board, int height, int length, int x, int y);
char applyRules(char currentState, int countLiveOfNeighbors);
void copyBoard(char*** target, char*** source);
int isLifeOrDead(char symbol);
void changeSpeedOfProgramm(int* delay, char inputKey);

int main() {
    int status = 0;
    if (gameOfLife(HEIGHT, LENGTH) != 0) {
        printf("n/a");
        status = -1;
    }
    return status;
}

int gameOfLife(int height, int length) {
    int delay = 500000, status = 0;
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    char **board = makeBoard(height, length), **newBoard = makeBoard(height, length), inputKey;
    if (board == NULL || newBoard == NULL)
        status = -1;
    else {
        initializeBoard(board, height, length);
    }
    if (status != 0 || freopen("/dev/tty", "r", stdin) == NULL) status = -1;
    while (status == 0 && ((inputKey = getch()) != 'q') && (inputKey != 'Q')) {
        clear();
        displayBoard(board, height, length);
        updateBoard(board, newBoard, height, length);
        copyBoard(&board, &newBoard);
        changeSpeedOfProgramm(&delay, inputKey);
        usleep(delay);
        refresh();
    }
    free(board);
    free(newBoard);
    endwin();
    return status;
}

char** makeBoard(int height, int length) {
    char** board = malloc(height * length * sizeof(char) + height * sizeof(char*));
    if (board != NULL) {
        char* intermediate = (char*)(board + height);
        for (int i = 0; i < height; i++) board[i] = intermediate + length * i;
    }
    return board;
}

void updateBoard(char** source, char** target, int height, int length) {
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < length; y++) {
            int liveNeighbors = countNeighbors(source, height, length, x, y);
            target[x][y] = applyRules(source[x][y], liveNeighbors);
        }
    }
}

void initializeBoard(char** board, int height, int length) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < length; j++) {
            board[i][j] = getch();
        }
        getch();
    }
}

void displayBoard(char** board, int height, int length) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < length; j++) {
            printw("%c", board[i][j]);
        }
        printw("\n");
    }
}

int isLifeOrDead(char symbol) { return symbol == '*'; }

int countNeighbors(char** board, int height, int length, int x, int y) {
    int life = 0, i = height + x, j = length + y;
    life += isLifeOrDead(board[(i - 1) % height][(j - 1) % length]);
    life += isLifeOrDead(board[(i - 1) % height][j % length]);
    life += isLifeOrDead(board[i % height][(j - 1) % length]);
    life += isLifeOrDead(board[i % height][(j + 1) % length]);
    life += isLifeOrDead(board[(i + 1) % height][j % length]);
    life += isLifeOrDead(board[(i + 1) % height][(j + 1) % length]);
    life += isLifeOrDead(board[(i + 1) % height][(j - 1) % length]);
    life += isLifeOrDead(board[(i - 1) % height][(j + 1) % length]);
    return life;
}

char applyRules(char currentState, int countLiveOfNeighbors) {
    char result;
    if ((currentState == ' ' && countLiveOfNeighbors == 3) ||
        (currentState == '*' && (countLiveOfNeighbors == 2 || countLiveOfNeighbors == 3)))
        result = '*';
    else
        result = ' ';
    return result;
}

void copyBoard(char*** target, char*** source) {
    char** temp = *source;
    *source = *target;
    *target = temp;
}

void changeSpeedOfProgramm(int* delay, char inputKey) {
    if (inputKey == '-' && *delay < 1000000)
        *delay += 100000;
    else if (inputKey == '+' && *delay > 100000)
        *delay -= 100000;
}