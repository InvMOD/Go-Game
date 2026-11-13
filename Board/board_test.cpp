#include "board.hpp"
#include "../lib/raylib.h"
#include "../UI Game/menu.hpp"

int main(){
    int screenWidth = 696;
    int screenHeight = 696;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Go Board Test");
    SetWindowMinSize(400, 400);
    SetTargetFPS(60);

    Board board;
    board.setTurn(1);
    board.updateLayout(screenWidth, screenHeight);
    bool menuOpen = true;

    while(!WindowShouldClose()){
        if (IsWindowResized()) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
            board.updateLayout(screenWidth, screenHeight);
        }

        Vector2 mouse = GetMousePosition();

        if (menuOpen) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                IsPlayButtonClicked(mouse, GetPlayButton(screenWidth, screenHeight))) {
                menuOpen = false;
            }
        } else {
            BoardPoint hoverPoint = board.ScreenToBoard(mouse);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                board.applyMove(hoverPoint);
            }

            if (IsKeyPressed(KEY_R)) {
                board.reset();
                board.setTurn(1);
            }
        }

        BeginDrawing();
            if (menuOpen) {
                DrawMenu(mouse, screenWidth, screenHeight);
            } else {
                ClearBackground(RAYWHITE);

                // Vẽ bàn cờ gỗ và lưới
                board.DrawBase();

                // Vẽ quân cờ mờ khi rê chuột (Ghost stone)
                BoardPoint hoverPoint = board.ScreenToBoard(mouse);
                Stone currentTurnStone = (board.currentTurn == 1) ? Stone::Black : Stone::White;
                board.DrawGhost(hoverPoint, currentTurnStone);

                // Vẽ các quân cờ đã đặt
                board.DrawStones();
                
                // Vẽ giao diện thông tin
                DrawText(board.currentTurn == static_cast<int>(Stone::Black) ? "Turn: BLACK" : "Turn: WHITE", 16, 16, 20, BLACK);
                DrawText("Press [R] to reset", screenWidth - 180, 16, 18, DARKGRAY);
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
