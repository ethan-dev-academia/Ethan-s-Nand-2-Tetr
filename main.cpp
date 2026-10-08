#include "raylib.h"

int cellSize = 25;
int width = cellSize * 10;
int offset = 125;

int board[20][10] = {};

int pieces[7][4][4] = {
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
};

int currentPiece = 0;
int active[4][4] = {};

int posX = 3;
int posY = 0;

int heldPiece = -1;
bool canHold = true;

int holdX = 383;
int holdY = 20;

void loadPiece(){
    for (int r = 0; r < 4; r++){
        for (int c = 0; c < 4; c++){
            active[r][c] = pieces[currentPiece][r][c];
        }
    }
}

bool exists(int newX, int newY){
    for (int r = 0; r < 4; r++){
        for (int c = 0; c < 4; c++){
            if (active[r][c] == 1){
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

bool spawnPiece(){
    loadPiece();
    posX = 3;
    posY = 0;
    return exists(posX, posY);
}

void nextPiece(){
    currentPiece++;
    if (currentPiece >= 7){
        currentPiece = 0;
    }
}

void rotatePiece(int x, int y){
    if (currentPiece == 1){
        return;
    }

    int size = 3;
    if (currentPiece == 0){
        size = 4;
    }

    int old[4][4];
    for (int r = 0; r < 4; r++){
        for (int c = 0; c < 4; c++){
            old[r][c] = active[r][c];
        }
    }

    int turned[4][4] = {};
    for (int r = 0; r < size; r++){
        for (int c = 0; c < size; c++){
            turned[r][c] = old[size - 1 - c][r];
        }
    }

    for (int r = 0; r < 4; r++){
        for (int c = 0; c < 4; c++){
            active[r][c] = turned[r][c];
        }
    }

    if (!exists(x, y)){
        for (int r = 0; r < 4; r++){
            for (int c = 0; c < 4; c++){
                active[r][c] = old[r][c];
            }
        }
    }
}

void lockPiece(int x, int y){
    for (int r = 0; r < 4; r++){
        for (int c = 0; c < 4; c++){
            if (active[r][c] == 1){
                board[y + r][x + c] = 1;
            }
        }
    }
}

void clearLines(){
    for (int row = 19; row >= 0; row--){
        bool full = true;
        for (int col = 0; col < 10; col++){
            if (board[row][col] == 0){
                full = false;
            }
        }

        if (full){
            for (int r = row; r > 0; r--){
                for (int col = 0; col < 10; col++){
                    board[r][col] = board[r - 1][col];
                }
            }
            for (int col = 0; col < 10; col++){
                board[0][col] = 0;
            }
            row++;
        }
    }
}

int main(){
    InitWindow(500, 500, "Tetr-8B");
    SetTargetFPS(60);
    int count = 0;

    bool run = true;
    bool gameOver = false;

    spawnPiece();

    while (run){
        if (!gameOver){
            if (IsKeyPressed(KEY_LEFT) && exists(posX - 1, posY)){
                posX--;
            }
            if (IsKeyPressed(KEY_RIGHT) && exists(posX + 1, posY)){
                posX++;
            }
            if (IsKeyPressed(KEY_DOWN) && exists(posX, posY + 1)){
                posY++;
            }
            if (IsKeyPressed(KEY_UP)){
                rotatePiece(posX, posY);
            }
            if (IsKeyPressed(KEY_C) && canHold){
                if (heldPiece == -1){
                    heldPiece = currentPiece;
                    nextPiece();
                } else {
                    int swap = heldPiece;
                    heldPiece = currentPiece;
                    currentPiece = swap;
                }
                canHold = false;
                count = 0;
                if (!spawnPiece()){
                    gameOver = true;
                }
            }
            if (IsKeyPressed(KEY_SPACE)){
                while (exists(posX, posY + 1)){
                    posY++;
                }
                count = 30;
            }

            count++;
            if (!gameOver && count >= 30){
                count = 0;

                if (exists(posX, posY + 1)){
                    posY++;
                } else {
                    lockPiece(posX, posY);
                    clearLines();
                    nextPiece();
                    canHold = true;
                    if (!spawnPiece()){
                        gameOver = true;
                    }
                }
            }
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

        if (!gameOver){
            for (int r = 0; r < 4; r++){
                for (int c = 0; c < 4; c++){
                    int pixX = offset + cellSize * (posX + c);
                    int pixY = cellSize * (posY + r);

                    if (active[r][c] == 1){
                        DrawRectangle(pixX, pixY, cellSize, cellSize, WHITE);
                    }
                }
            }
        }

        DrawRectangleLines(holdX, holdY, cellSize * 4 + 10, cellSize * 4 + 10, WHITE);
        if (heldPiece != -1){
            for (int r = 0; r < 4; r++){
                for (int c = 0; c < 4; c++){
                    if (pieces[heldPiece][r][c] == 1){
                        DrawRectangle(holdX + 5 + cellSize * c, holdY + 5 + cellSize * r, cellSize, cellSize, WHITE);
                    }
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
