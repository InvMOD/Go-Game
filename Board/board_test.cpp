#include "board.hpp"
#include "../lib/raylib.h"
#include "../UI Game/menu.hpp"

int main(){
    const int screenWidth = 696;
    const int screenHeight = 696;

    InitWindow(screenWidth, screenHeight, "Go Bocard Test");
    SetTargetFPS(60);

    Board board;
    board.setTurn(1);
    bool menuOpen = true;

    while(!WindowShouldClose()){
        if(menuOpen){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
               IsPlayButtonClicked(GetMousePosition(), GetPlayButton())){
                menuOpen = false;
            }
        } else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mouse = GetMousePosition();
            board.applyMove(board.ScreenToBoard(mouse));
        } else if(IsKeyPressed(KEY_R)){
            board.reset();
            board.setTurn(1);
        }

        BeginDrawing();
            if(menuOpen){
                DrawMenu(GetMousePosition());
            } else {
                board.DrawBase();
                board.DrawStones();
                DrawText(board.currentTurn == static_cast<int>(Stone::Black) ? "Turn: BLACK" : "Turn: WHITE", 10, 10, 18, BLACK);
                DrawText("Press [R] to reset", 350, 10, 16, DARKGRAY);
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}