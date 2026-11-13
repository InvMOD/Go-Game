#include "board.hpp"
#include "../lib/raylib.h"

int main(){
    int screenWidth = 696;
    int screenHeight = 696;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "Go Board Test");
    SetWindowMinSize(400, 400);

    SetTargetFPS(60);

    Board board;
    board.setTurn(1);

    while(!WindowShouldClose()){
        if (IsWindowResized()) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mouse = GetMousePosition();
            board.applyMove(board.ScreenToBoard(mouse));
        }

        if(IsKeyPressed(KEY_R)){
            board.reset();
            board.setTurn(1);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            board.DrawBase();
            board.DrawStones();
            
            DrawText(board.currentTurn == static_cast<int>(Stone::Black) ? "Turn: BLACK" : "Turn: WHITE", 10, 10, 18, BLACK);
            DrawText("Press [R] to reset", screenWidth - 200, 10, 16, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}