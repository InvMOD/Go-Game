#include <bits/stdc++.h>

#include "board.hpp"

#define FOR(i, a, b) for(int i = (a); i <= (b); i++)

Vector2 Board::BoardToScreen(int r, int c) const{
    return {
        static_cast<float>(margin + r * cellSize),
        static_cast<float>(margin + c * cellSize)
    };
}

BoardPoint Board::ScreenToBoard(Vector2 mousePos) const{
    int r = std::round((mousePos.x - margin) / (1.0l * cellSize));
    int c = std::round((mousePos.y - margin) / (1.0l * cellSize));
    return {r, c};
}

void Board::DrawBase() const{
    ClearBackground(woodColor);
    DrawRectangleLines(margin - 12, margin - 12, gridSpan + 24, gridSpan + 24, lineDarkColor);

    FOR(i, 0, BOARD_SIZE - 1){
        int offset = margin + i * cellSize;
        DrawLine(margin, offset, margin + gridSpan, offset, lineDarkColor);
        DrawLine(offset, margin, offset, margin + gridSpan, lineDarkColor);
    }

    const BoardPoint starPoints[] = {
        {2, 2}, {2, 6},
        {4, 4}, 
        {6, 2}, {6, 6}
    };

    for(const auto& p : starPoints){
        Vector2 pos = BoardToScreen(p.r, p.c);
        DrawCircleV(pos, 4.0f, lineDarkColor);
    }
}

void Board::DrawStones() const{
    float radius = cellSize * 0.46f;
    FOR(r, 0, BOARD_SIZE - 1){
        FOR(c, 0, BOARD_SIZE - 1){
            if(grid[r][c] == Stone::Empty) continue;

            Vector2 center = BoardToScreen(r, c);
            DrawCircle(center.x + 2, center.y + 2, radius, ColorAlpha(BLACK, 0.25f));

            if(grid[r][c] == Stone::Black){
                DrawCircleV(center, radius, blackStoneColor);
                DrawCircleLines(center.x, center.y, radius, {60, 60, 60, 225});
            }
            else{
                DrawCircleV(center, radius, whiteStoneColor);
                DrawCircleLines(center.x, center.y, radius, {200, 200, 200, 225});              
            }
        }
    }
}
