#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include<fstream>
using namespace std;

const int SCREEN_WIDTH = 90;
const int SCREEN_HEIGHT = 26;
const int WIN_WIDTH = 70;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
bool enemyFlag[3];
char car[4][4] = {' ', '+', '+', ' ',
                  '+', '+', '+', '+',
                  ' ', '+', '+', ' ',
                  '+', '+', '+', '+'};

int carPos = WIN_WIDTH / 2;
int carVerticalPos = SCREEN_HEIGHT - 5; // Initial vertical position of the car
int score = 0;

void gotoxy(int x, int y) ;
void setcursor(bool visible, DWORD size);
void genEnemy(int ind);
void drawEnemy(int ind);
void drawBorder();
void eraseEnemy(int ind);
void resetEnemy(int ind);
void drawCar();
void eraseCar();
int collision();
void play();
void gameover();
void updateScore();
void instructions();

int main() {
        system("color 0A");
    setcursor(0, 0);
    srand((unsigned)time(NULL));
    int op;
    do {
        system("cls");
        gotoxy(10, 5);
        cout << "-----------------------";
        gotoxy(10, 6);
        cout << "|--------Car Game-----|";
        gotoxy(10, 7);
        cout << "-----------------------";
        gotoxy(10, 9);
        cout << "1. Start Game ";
        gotoxy(10, 10);
        cout << "2. Instructions ";
        gotoxy(10, 11);
        cout << "3. Quit";
        gotoxy(10, 13);
        cout << "Select Option = ";
        cin >> op;

        if (op == 1) {
            play();
        } else if (op == 2) {
            instructions();
        } else if (op == 3) {
            exit(0);
        }

    } while (1);
    return 0;
}
///  Fucntions start from here

void play() {
    carPos = -1 + WIN_WIDTH / 2;
    carVerticalPos = SCREEN_HEIGHT - 5; // Reset car to the bottom
    score = 0;
    for (int i = 0; i < 3; i++) {
        resetEnemy(i);
    }
    system("cls");
    drawBorder();
    updateScore();

    gotoxy(WIN_WIDTH + 7, 2);
    cout << "Car Game";
    gotoxy(WIN_WIDTH + 7, 3);
    cout << "------------------------";

    gotoxy(WIN_WIDTH + 7, 12);
    cout << "Control";
    gotoxy(WIN_WIDTH + 7, 13);
    cout << "------------------------";
    gotoxy(WIN_WIDTH + 2, 14);
    cout << "Left Arrow- Left";
    gotoxy(WIN_WIDTH + 2, 15);
    cout << "Right Arrow- Right";
    gotoxy(WIN_WIDTH + 2, 16);
    cout << "Up Arrow- Up";
    gotoxy(WIN_WIDTH + 2, 17);
    cout << "Down Arrow- Down";

    gotoxy(WIN_WIDTH + 18, 5);
    cout << "Press any key to Start";
    getch();
    gotoxy(WIN_WIDTH + 7, 2);
    cout << "                         ";

    int currentEnemy = 0; // Index of the current enemy

    while (1) {
            if (GetAsyncKeyState(VK_LEFT)) {
                if (carPos > 18) {
                    carPos -= 4;
                }
            }
            if (GetAsyncKeyState(VK_RIGHT)) {
                if (carPos < WIN_WIDTH - 22) { // Adjusted boundary to prevent going too far right
                    carPos += 4;
                }
            }
            if (GetAsyncKeyState(VK_UP)) {
                if (carVerticalPos > 0) {
                    carVerticalPos--;
                }
            }
            if (GetAsyncKeyState(VK_DOWN)) {
                if (carVerticalPos < SCREEN_HEIGHT - 4) {
                    carVerticalPos++;
                }
            }
            if (GetAsyncKeyState(VK_ESCAPE)) {
                break;
            }
        

        drawCar();
        for (int i = 0; i < 3; i++) {
            drawEnemy(i);
        }

        if (collision() == 1) {
            gameover();
            return;
        }

        Sleep(100); // Adjust sleep duration to control game speed
        eraseCar();
        
        for (int i = 0; i < 3; i++) {
            eraseEnemy(i);
        }

        for (int i = 0; i < 3; i++) {
            if (enemyY[i] >= SCREEN_HEIGHT - 4) {
                resetEnemy(i); // Reset the current enemy
                score++; // Increment the score when an enemy passes the bottom
                updateScore();
            } else {
                enemyY[i] += 1; // Move the current enemy down
            }
        }
    }
}

void instructions() {
    system("cls");
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 1);
    cout << "Instructions";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2);
    cout << "----------------------";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 1);
    cout << "Avoid cars by moving left to right";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 2);
    cout << "Press 'A' to Move Left";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 3);
    cout << "Press 'D' to Move Right";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 4);
    cout << "Press 'W' to Move Up";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 5);
    cout << "Press 'S' to Move Down";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 7);
    cout << "Press Escape to Exit";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 9);
    cout << "Press any key to go back to the menu";
    getch();
}

void updateScore() {
    gotoxy(WIN_WIDTH + 7, 5);
    cout << "Score: " << score << endl;
 
}

void gameover() {
system("cls");
  int highScore=0;

  
    fstream fs;
    // Read the score
    fs.open("Score.txt", ios::in);
    fs>>highScore;
    fs.close();
  
//////////////////////////////////////
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 1);
    cout << "------------------------";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2);
    cout << "|------Game Over-------|";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 1);
    cout << "------------------------";
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 2);

/// display score 
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 3);
    cout << "Your Score is = " << score << endl;
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 4);
    cout << "Your HighScore was = " << highScore << endl;
// checking score in file and updating it 
    if (score > highScore)
    {
        fstream fs;
        // update the score
    fs.open("Score.txt", ios::out);
     fs<<score;
    fs.close();
    // write read again
     fs.open("Score.txt", ios::in);
     fs>>highScore;
    fs.close();
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 5);
    cout << "Your New HighScore is = " << highScore << endl;
    }

    fs.open("Score.txt",ios::out);
    fs<<score;
    fs.close();

    cout << "Do You want to play again (Y/N ) : ";
    char y;
    cin>>y;
    if (y=='Y' || y=='y')
    {
        play();
    }
}


int collision() {
    for (int ind = 0; ind < 3; ind++) {
        if (enemyFlag[ind] == true) {
            if (enemyY[ind] + 4 >= carVerticalPos && enemyY[ind] <= carVerticalPos + 3) {
                if (enemyX[ind] + 4 - carPos >= 0 && enemyX[ind] < carPos + 4) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void eraseCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carPos, i + carVerticalPos);
            cout << " ";
        }
    }
}

void drawCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carPos, i + carVerticalPos);
            cout << car[i][j];
        }
    }
}

void resetEnemy(int ind) {
    eraseEnemy(ind);
    enemyY[ind] = 1;

    // Generate a new random position within acceptable boundaries
    enemyX[ind] = 5 + rand() % (WIN_WIDTH - 10);

    // Ensure the generated position is within acceptable range
    while (enemyX[ind] <= 17 || enemyX[ind] >= WIN_WIDTH - 21) {
        enemyX[ind] = 5 + rand() % (WIN_WIDTH - 10);
    }

    enemyFlag[ind] = true;
}

void eraseEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyX[ind], enemyY[ind]);
        cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 1);
        cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 2);
        cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 3);
        cout << "    ";
    }
}

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
    if (size == 0)
        size = 20;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}



void drawBorder() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < 17; j++) {
            gotoxy(0 + j, i);
            cout << "+";
            gotoxy(WIN_WIDTH - j, i);
            cout << "+";
        }
    }

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        gotoxy(SCREEN_WIDTH, i);
        cout << "+";
    }
}

void genEnemy(int ind) {
    enemyY[ind] = 1;
    enemyX[ind] = 5 + rand() % (WIN_WIDTH - 10);
    enemyFlag[ind] = true;
}

void drawEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyX[ind], enemyY[ind]);
        cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 1);
        cout << " ** ";
        gotoxy(enemyX[ind], enemyY[ind] + 2);
        cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 3);
        cout << " **";
    }
}