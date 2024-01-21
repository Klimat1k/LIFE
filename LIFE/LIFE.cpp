/**
*
* Solution to course project # 2
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Boril Vasilev
* @idnumber 0MI0600369
* @compiler VC
*
* <main file>
*
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int START_MENU_HEIGHT = 4;
const int INGAME_MENU_HEIGHT = 8;
const int GAME_Y = 0;
const int GAME_X = 13;
const int MAX_ROWS = 26;
const int MAX_COLUMNS = 82;
const int MAX_DISPLAY_HEIGHT = MAX_ROWS + GAME_Y;
const int MAX_DISPLAY_WIDTH = MAX_COLUMNS + GAME_X;
const char FORWARD[] = "FORWARD";
const char RESIZE[] = "RESIZE";
const char TOGGLE[] = "TOGGLE";
const char CLEAR[] = "CLEAR";
const char RANDOMIZE[] = "RANDOMIZE";
const char SAVE[] = "SAVE";
const char END[] = "END";
const char NEW_GAME[] = "NEW GAME";
const char LOAD_FILE[] = "LOAD FILE";
const char EXIT[] = "EXIT";

void initState(bool gameState[MAX_ROWS][MAX_COLUMNS]) {
    for (int y = 0; y < MAX_ROWS; y++) {
        for (int x = 0; x < MAX_COLUMNS; x++) {
            gameState[y][x] = false;
        }
    }
}

void clearDisplay(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], int y0, int x0, int height, int width) {
    for (int y = y0; y < height + y0; y++) {
        for (int x = x0; x < width + x0; x++) {
            display[y][x] = ' ';
        }
    }
}

void initDisplay(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH]) {
    clearDisplay(display, 0, 0, MAX_DISPLAY_HEIGHT, MAX_DISPLAY_WIDTH);
}

void printDisplay(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], int rows, int columns, bool ingame) {
    int height = ingame ? max(GAME_Y + rows, INGAME_MENU_HEIGHT + 1) : START_MENU_HEIGHT + 1;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < GAME_X + columns; x++) {
            cout << display[y][x];
        }
        cout << endl;
    }
}

void initStartMenu(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH]) {
    for (int x = 0; x < size(NEW_GAME) - 1; x++) {
        display[0][x] = NEW_GAME[x];
    }
    for (int x = 0; x < size(LOAD_FILE) - 1; x++) {
        display[1][x] = LOAD_FILE[x];
    }
    for (int x = 0; x < size(EXIT) - 1; x++) {
        display[2][x] = EXIT[x];
    }
    for (int x = 0; x < GAME_X - 3; x++) {
        display[3][x] = '_';
    }
    for (int y = 0; y < START_MENU_HEIGHT; y++) {
        display[y][GAME_X - 3] = '|';
    }

}

void initInGameMenu(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH]) {
    for (int x = 0; x < size(FORWARD) - 1; x++) {
        display[0][x] = FORWARD[x];
    }
    for (int x = 0; x < size(RESIZE) - 1; x++) {
        display[1][x] = RESIZE[x];
    }
    for (int x = 0; x < size(TOGGLE) - 1; x++) {
        display[2][x] = TOGGLE[x];
    }
    for (int x = 0; x < size(CLEAR) - 1; x++) {
        display[3][x] = CLEAR[x];
    }
    for (int x = 0; x < size(RANDOMIZE) - 1; x++) {
        display[4][x] = RANDOMIZE[x];
    }
    for (int x = 0; x < size(SAVE) - 1; x++) {
        display[5][x] = SAVE[x];
    }
    for (int x = 0; x < size(END) - 1; x++) {
        display[6][x] = END[x];
    }
    for (int x = 0; x < GAME_X - 3; x++) {
        display[7][x] = '_';
    }
    for (int y = 0; y < INGAME_MENU_HEIGHT; y++) {
        display[y][GAME_X - 3] = '|';
    }
}

void updateGameDisplay(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns) {
    clearDisplay(display, GAME_Y, GAME_X - 2, MAX_ROWS, MAX_COLUMNS + 2);
    for (int y = 1; y < rows - 1; y++) {
        for (int x = 1; x < columns - 1; x++) {
            if (gameState[y][x]) {
                display[GAME_Y + y][GAME_X + x] = '@';
            }
            else {
                display[GAME_Y + y][GAME_X + x] = '-';
            }
        }
    }

    display[GAME_Y][GAME_X + 1] = '1';
    if ((columns - 2) < 10) {
        display[GAME_Y][GAME_X + columns - 2] = columns - 2 + '0';
    }
    else {
        display[GAME_Y][GAME_X + columns - 2] = (columns - 2) / 10 + '0';
        display[GAME_Y][GAME_X + columns - 1] = (columns - 2) % 10 + '0';
    }
    display[GAME_Y + 1][GAME_X - 1] = '1';
    if ((rows - 2) < 10) {
        display[GAME_Y + rows - 2][GAME_X - 1] = rows - 2 + '0';
    }
    else {
        display[GAME_Y + rows - 2][GAME_X - 2] = (rows - 2) / 10 + '0';
        display[GAME_Y + rows - 2][GAME_X - 1] = (rows - 2) % 10 + '0';
    }
}

int localLiveCellCount(bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns, int y0, int x0) {
    int count = 0;
    for (int y = y0 - 1; y < y0 - 1 + 3; y++) {
        for (int x = x0 - 1; x < x0 - 1 + 3; x++) {
            if (y >= 0 && x >= 0 && y < rows && x < columns && !(y == y0 && x == x0)) {
                if (gameState[y][x]) {
                    count++;
                }
            }
        }
    }
    return count;
}

void shiftState(bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns, bool down, bool right) {
    bool prevState[MAX_ROWS][MAX_COLUMNS];
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            prevState[y][x] = gameState[y][x];
        }
    }
    initState(gameState);
    for (int y = 0; y < rows - down; y++) {
        for (int x = 0; x < columns - right; x++) {
            gameState[y + down][x + right] = prevState[y][x];
        }
    }
}

void onNextState(bool gameState[MAX_ROWS][MAX_COLUMNS], int& rows, int& columns) {
    bool bottom = false;
    bool right = false;
    bool top = false;
    bool left = false;
    bool prevState[MAX_ROWS][MAX_COLUMNS];
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            prevState[y][x] = gameState[y][x];
        }
    }
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            if (prevState[y][x]) {
                if (localLiveCellCount(prevState, rows, columns, y, x) < 2 || localLiveCellCount(prevState, rows, columns, y, x) > 3) {
                    gameState[y][x] = false;
                }
            }
            else {
                if (localLiveCellCount(prevState, rows, columns, y, x) == 3) {
                    gameState[y][x] = true;

                    if (y == rows - 1) bottom = true;
                    if (x == columns - 1) right = true;
                    if (y == 0) top = true;
                    if (x == 0) left = true;
                }
            }
        }
    }

    if (bottom) {
        if (rows + 1 <= MAX_ROWS) {
            rows++;
        }
    }
    if (right) {
        if (columns + 1 <= MAX_COLUMNS) {
            columns++;
        }
    }
    if (top) {
        if (rows + 1 <= MAX_ROWS) {
            rows++;
            shiftState(gameState, rows, columns, true, false);
        }
    }
    if (left) {
        if (columns + 1 <= MAX_COLUMNS) {
            columns++;
            shiftState(gameState, rows, columns, false, true);
        }
    }
}

void onForwardInput(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int& rows, int& columns) {
    cout << endl;
    onNextState(gameState, rows, columns);
    updateGameDisplay(display, gameState, rows, columns);
    printDisplay(display, rows, columns, true);
}

void onResizeInput(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int& rows, int& columns) {
    cout << "Enter new height and width: ";
    cin >> rows >> columns;
    rows += 2;
    columns += 2;
    if (rows < 3 || rows > MAX_ROWS || columns < 3 || columns > MAX_COLUMNS) {
        cout << "Invalid dimensions";
        cout << endl << endl;
        return;
    }
    cout << endl;

    bool prevState[MAX_ROWS][MAX_COLUMNS];
    for (int y = 1; y < rows - 1; y++) {
        for (int x = 1; x < columns - 1; x++) {
            prevState[y][x] = gameState[y][x];
        }
    }
    initState(gameState);
    for (int y = 1; y < rows - 1; y++) {
        for (int x = 1; x < columns - 1; x++) {
            gameState[y][x] = prevState[y][x];
        }
    }
    updateGameDisplay(display, gameState, rows, columns);
    printDisplay(display, rows, columns, true);
}

void onToggleInput(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns) {
    int y, x;
    cout << "Enter y and x coordinates: ";
    cin >> y >> x;
    if (y < 1 || y > rows - 2 || x < 1 || x > columns - 2) {
        cout << "Invalid coordinates";
        cout << endl << endl;
        return;
    }
    cout << endl;

    if (gameState[y][x]) {
        gameState[y][x] = false;
    }
    else {
        gameState[y][x] = true;
    }
    updateGameDisplay(display, gameState, rows, columns);
    printDisplay(display, rows, columns, true);
}

void onClearInput(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns) {
    cout << endl;
    initState(gameState);
    updateGameDisplay(display, gameState, rows, columns);
    printDisplay(display, rows, columns, true);
}

void onRandomizeInput(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns) {
    int n;
    cout << "Enter probability for live cell: 1/";
    cin >> n;
    cout << endl;
    initState(gameState);
    if (n != 0) {
        for (int y = 1; y < rows - 1; y++) {
            for (int x = 1; x < columns - 1; x++) {
                int randomValue = rand() % n + 1;
                gameState[y][x] = (randomValue == 1);
            }
        }
    }
    updateGameDisplay(display, gameState, rows, columns);
    printDisplay(display, rows, columns, true);
}

void onSaveInput(bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns) {
    int minRow = rows;
    int minCol = columns;
    int maxRow = -1;
    int maxCol = -1;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            if (gameState[y][x]) {
                minRow = min(minRow, y);
                maxRow = max(maxRow, y);
                minCol = min(minCol, x);
                maxCol = max(maxCol, x);
            }
        }
    }
    if (minRow > maxRow && minCol > maxCol) {
        cout << "No live cells to save" << endl << endl;
        return;
    }
    string fileName;
    cout << "Enter save file name: ";
    cin >> fileName;
    cout << endl;
    ofstream saveFile(fileName);
    if (saveFile.is_open()) {
        saveFile << maxRow + 1 - minRow + 2 << " " << maxCol + 1 - minCol + 2 << endl;
        for (int y = minRow - 1; y < maxRow + 2; y++) {
            for (int x = minCol - 1; x < maxCol + 2; x++) {
                saveFile << (gameState[y][x] ? '1' : '0') << " ";
            }
            saveFile << endl;
        }
        saveFile.close();
    }
}

bool onLoadInput(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int& rows, int& columns) {
    string fileName;
    cout << "Enter save file name: ";
    cin >> fileName;
    ifstream loadFile(fileName);
    if (loadFile.is_open()) {
        loadFile >> rows >> columns;

        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                int cellValue;
                loadFile >> cellValue;
                gameState[y][x] = (cellValue == 1);
            }
        }
        loadFile.close();
    }
    else {
        cout << "Unable to open save file" << endl << endl;
        return false;
    }
    return true;
}

bool onStartup(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns);

void onNewGame(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns) {
    initDisplay(display);
    initInGameMenu(display);
    updateGameDisplay(display, gameState, rows, columns);
    printDisplay(display, rows, columns, true);
    string input;
    getline(cin, input);
    while (input != "END" && input != "end") {
        if (input == "FORWARD" || input == "forward") {
            onForwardInput(display, gameState, rows, columns);
        }
        else if (input == "RESIZE" || input == "resize") {
            onResizeInput(display, gameState, rows, columns);
        }
        else if (input == "TOGGLE" || input == "toggle") {
            onToggleInput(display, gameState, rows, columns);
        }
        else if (input == "CLEAR" || input == "clear") {
            onClearInput(display, gameState, rows, columns);
        }
        else if (input == "RANDOMIZE" || input == "randomize") {
            onRandomizeInput(display, gameState, rows, columns);
        }
        else if (input == "SAVE" || input == "save") {
            onSaveInput(gameState, rows, columns);
        }
        getline(cin, input);
    }
    if (input == "END" || input == "end") {
        cout << endl;
        onStartup(display, gameState, rows, columns);
    }
}

bool onStartup(char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH], bool gameState[MAX_ROWS][MAX_COLUMNS], int rows, int columns) {
    initDisplay(display);
    initStartMenu(display);
    printDisplay(display, rows, columns, false);
    string input;
    getline(cin, input);
    while (input != "EXIT" && input != "exit") {
        if (input == "NEW GAME" || input == "new game") {
            cout << endl;
            initState(gameState);
            onNewGame(display, gameState, rows, columns);
            break;
        }
        else if (input == "LOAD FILE" || input == "load file") {
            if (onLoadInput(display, gameState, rows, columns)) {
                cout << endl;
                onNewGame(display, gameState, rows, columns);
                break;
            }
        }
        getline(cin, input);
    }
    if (input == "EXIT" || input == "exit") {
        return false;
    }
    return true;
}

int main() {
    srand(time(0));
    int rows = 10;
    int columns = 18;
    bool gameState[MAX_ROWS][MAX_COLUMNS];
    initState(gameState);
    char display[MAX_DISPLAY_HEIGHT][MAX_DISPLAY_WIDTH];

    if (!onStartup(display, gameState, rows, columns)) {
        return 0;
    }
}