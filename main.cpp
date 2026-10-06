// I haven't written straight C++ in a long time.
// This is a test in C++ for the graphical rendering system under 8 bit constraints.

#include "raylib.h"


int cellSize = 25;
int width = cellSize * 10;
int offset = 125;

int board[20][10] = {};
int pieceSQR[4][4] = {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

bool exists(int newX, int newY){
    for (int r = 0; r < 4; r++){
        for (int c = 0; c < 4; c++){
            if (pieceSQR[r][c] == 1){
                int boardX = newX + c;
                int boardY = newY + r;
                if (boardX < 0 || boardX >= 10 || boardY < 0 || boardY >= 20){
                    return false;
                }
                if (board[boardY][boardX] == 1){
                    return false;
                }
            }
        }
    }
    return true;
}

int main(){
    InitWindow(500, 500, "Tetr-8B");
    SetTargetFPS(60);
    int count = 0;
    
    bool run = true;
    
    int posX = 2;
    int posY = 3;

    while (run){
        if (IsKeyPressed(KEY_LEFT) && exists(posX - 1, posY)){
            posX--;
        }
        if (IsKeyPressed(KEY_RIGHT) && exists(posX + 1, posY)){
            posX++;
        }
        if (IsKeyPressed(KEY_DOWN) && exists(posX, posY + 1)){
            posY++;
        }
        
        count++;
        if (count >= 30 && exists(posX, posY + 1)){
            posY++;
            count = 0;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        
        for (int x = 0; x < 10; x++){
            for (int y = 0; y < 20; y++){
                if (board[y][x] == 1){
                    DrawRectangle(offset + cellSize * x, cellSize * y, cellSize, cellSize, WHITE);
                }
                else{
                    DrawRectangleLines(offset + cellSize * x, cellSize * y, cellSize, cellSize, WHITE);
                }
            }
        }

        for (int r = 0; r < 4; r++){
            for (int c = 0; c < 4; c++){
                int pixX = offset + cellSize * (posX + c);
                int pixY = cellSize * (posY + r);

                if (pieceSQR[r][c] == 1){
                    DrawRectangle(pixX, pixY, cellSize, cellSize, WHITE);
                }
            }
        }

        EndDrawing();

        if (WindowShouldClose()){
            run = false;
        }
    }

    CloseWindow();
    return 0;
}
