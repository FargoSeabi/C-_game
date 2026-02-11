#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <ctime>
using namespace std;

// Game settings
const int width = 30;
const int height = 20;

bool gameOver;
bool wrapMode = false;
int difficultySpeed = 100;

int x, y, fruitX, fruitY, score, highScore;
int tailX[200], tailY[200];
int nTail;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void LoadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        highScore = 0;
    }
}

void SaveHighScore() {
    ofstream file("highscore.txt");
    file << highScore;
    file.close();
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    score = 0;
    nTail = 0;
    srand(time(0));
    fruitX = rand() % width;
    fruitY = rand() % height;
}

void Draw() {
    system("cls");

    SetColor(11);
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            if (j == 0)
                cout << "#";

            if (i == y && j == x) {
                SetColor(10);
                cout << "O";
                SetColor(7);
            }
            else if (i == fruitY && j == fruitX) {
                SetColor(12);
                cout << "F";
                SetColor(7);
            }
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        SetColor(2);
                        cout << "o";
                        SetColor(7);
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << "   High Score: " << highScore << endl;
    cout << "Press P to Pause | X to Exit" << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': dir = LEFT; break;
        case 'd': dir = RIGHT; break;
        case 'w': dir = UP; break;
        case 's': dir = DOWN; break;
        case 'x': gameOver = true; break;
        case 'p':
            cout << "\nGame Paused. Press any key to continue...";
            _getch();
            break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    default: break;
    }

    // Wrap mode
    if (wrapMode) {
        if (x >= width) x = 0;
        else if (x < 0) x = width - 1;
        if (y >= height) y = 0;
        else if (y < 0) y = height - 1;
    }
    else {
        if (x >= width || x < 0 || y >= height || y < 0)
            gameOver = true;
    }

    // Tail collision
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    // Eat fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;

        if (score > highScore) {
            highScore = score;
            SaveHighScore();
        }
    }
}

void ShowMenu() {
    cout << "==== SNAKE GAME ====" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cout << "Select Difficulty: ";

    int choice;
    cin >> choice;

    switch (choice) {
    case 1: difficultySpeed = 120; break;
    case 2: difficultySpeed = 80; break;
    case 3: difficultySpeed = 50; break;
    default: difficultySpeed = 100;
    }

    cout << "Enable Wrap Mode? (1 = Yes, 0 = No): ";
    cin >> wrapMode;
}

int main() {
    LoadHighScore();
    ShowMenu();
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(difficultySpeed);
    }

    cout << "\nGame Over!" << endl;
    cout << "Final Score: " << score << endl;
    system("pause");
    return 0;
}

