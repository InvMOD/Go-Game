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
    bool showScore = false;

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
                showScore = false;
            }

            if (IsKeyPressed(KEY_S)) {
                showScore = !showScore;
            }
        }

        BeginDrawing();
            if (menuOpen) {
                DrawMenu(mouse, screenWidth, screenHeight);
            } else {
                ClearBackground(RAYWHITE);

                // Vẽ bàn cờ gỗ và lưới
                board.DrawBase();

                if (showScore) {
                    board.DrawTerritory();
                }

                // Vẽ quân cờ mờ khi rê chuột (Ghost stone)
                BoardPoint hoverPoint = board.ScreenToBoard(mouse);
                Stone currentTurnStone = (board.currentTurn == 1) ? Stone::Black : Stone::White;
                board.DrawGhost(hoverPoint, currentTurnStone);

                // Vẽ các quân cờ đã đặt
                board.DrawStones();
                
                // Vẽ giao diện thông tin
                DrawText(board.currentTurn == static_cast<int>(Stone::Black) ? "Turn: BLACK" : "Turn: WHITE", 16, 14, 20, BLACK);
                DrawText(TextFormat("Captures - B: %d | W: %d", board.blackCaptures, board.whiteCaptures), 16, 36, 15, DARKGRAY);
                DrawText("Press [S] Score | [R] Reset", screenWidth - 250, 16, 18, DARKGRAY);

                if (showScore) {
                    BoardScore score = board.calculateScore();
                    int boxW = 340;
                    int boxH = 76;
                    int boxX = (screenWidth - boxW) / 2;
                    int boxY = screenHeight - boxH - 10;

                    DrawRectangle(boxX, boxY, boxW, boxH, ColorAlpha(WHITE, 0.92f));
                    DrawRectangleLines(boxX, boxY, boxW, boxH, DARKGRAY);

                    DrawText(TextFormat("Black: %d (Territory: %d, Captures: %d)", score.blackTotal, score.blackTerritory, score.blackCaptures), boxX + 12, boxY + 10, 15, BLACK);
                    DrawText(TextFormat("White: %d (Territory: %d, Captures: %d)", score.whiteTotal, score.whiteTerritory, score.whiteCaptures), boxX + 12, boxY + 30, 15, DARKGRAY);

                    const char* resultText = "Tied game!";
                    Color resultColor = DARKGRAY;
                    if (score.blackTotal > score.whiteTotal) {
                        resultText = TextFormat("Black leads by %d pts!", score.blackTotal - score.whiteTotal);
                        resultColor = BLACK;
                    } else if (score.whiteTotal > score.blackTotal) {
                        resultText = TextFormat("White leads by %d pts!", score.whiteTotal - score.blackTotal);
                        resultColor = DARKBLUE;
                    }
                    DrawText(resultText, boxX + 12, boxY + 50, 15, resultColor);
                }
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
