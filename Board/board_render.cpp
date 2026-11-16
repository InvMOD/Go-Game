#include "board.hpp"
#include <algorithm>
#include <cmath>

void Board::updateLayout(int screenWidth, int screenHeight) {
    float topMargin = 50.0f;
    float bottomMargin = 25.0f;
    float sideMargin = 25.0f;

    float availW = (float)screenWidth - 2.0f * sideMargin;
    float availH = (float)screenHeight - topMargin - bottomMargin;
    float availSize = std::min(availW, availH);
    if (availSize < 50.0f) availSize = 50.0f;

    cellSize = availSize / (BOARD_SIZE + 0.3f);
    gridSpan = (BOARD_SIZE - 1) * cellSize;
    boardPadding = cellSize * 0.65f;

    float centerX = (float)screenWidth * 0.5f;
    float centerY = topMargin + availH * 0.5f;

    offsetX = centerX - gridSpan * 0.5f;
    offsetY = centerY - gridSpan * 0.5f;
}

Vector2 Board::BoardToScreen(int r, int c) const {
    return {
        offsetX + c * cellSize,
        offsetY + r * cellSize
    };
}

BoardPoint Board::ScreenToBoard(Vector2 mousePos) const {
    int c = std::round((mousePos.x - offsetX) / cellSize);
    int r = std::round((mousePos.y - offsetY) / cellSize);
    return {r, c};
}

void Board::DrawBase() const {
    Rectangle boardRect = {
        offsetX - boardPadding,
        offsetY - boardPadding,
        gridSpan + 2.0f * boardPadding,
        gridSpan + 2.0f * boardPadding
    };

    float shadowOffset = std::max(2.0f, cellSize * 0.08f);
    DrawRectangleRec({boardRect.x + shadowOffset, boardRect.y + shadowOffset, boardRect.width, boardRect.height}, ColorAlpha(BLACK, 0.2f));
    DrawRectangleRec(boardRect, woodColor);

    float borderThickness = std::max(1.5f, cellSize * 0.04f);
    DrawRectangleLinesEx(boardRect, borderThickness, lineDarkColor);

    float lineThickness = std::max(1.0f, cellSize * 0.025f);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        float x = offsetX + i * cellSize;
        float y = offsetY + i * cellSize;
        DrawLineEx({offsetX, y}, {offsetX + gridSpan, y}, lineThickness, lineDarkColor);
        DrawLineEx({x, offsetY}, {x, offsetY + gridSpan}, lineThickness, lineDarkColor);
    }

    const BoardPoint starPoints[] = {
        {2, 2}, {2, 6},
        {4, 4},
        {6, 2}, {6, 6}
    };

    float starRadius = std::max(2.0f, cellSize * 0.07f);
    for (const auto& p : starPoints) {
        Vector2 pos = BoardToScreen(p.r, p.c);
        DrawCircleV(pos, starRadius, lineDarkColor);
    }
}

void Board::DrawStones() const {
    float radius = cellSize * 0.46f;
    float shadowOffset = std::max(1.5f, cellSize * 0.05f);

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (grid[r][c] == Stone::Empty) continue;

            Vector2 center = BoardToScreen(r, c);
            DrawCircle(center.x + shadowOffset, center.y + shadowOffset, radius, ColorAlpha(BLACK, 0.25f));

            if (grid[r][c] == Stone::Black) {
                DrawCircleV(center, radius, blackStoneColor);
                DrawCircleLines(center.x, center.y, radius, {60, 60, 60, 255});
            } else {
                DrawCircleV(center, radius, whiteStoneColor);
                DrawCircleLines(center.x, center.y, radius, {200, 200, 200, 255});
            }

            if (r == lastMove.r && c == lastMove.c) {
                Color markColor = (grid[r][c] == Stone::Black) ? RED : MAROON;
                DrawCircleV(center, radius * 0.22f, markColor);
            }
        }
    }
}

void Board::DrawGhost(BoardPoint hoverPoint, Stone turnStone) const {
    if (!init(hoverPoint.r, hoverPoint.c)) return;
    if (grid[hoverPoint.r][hoverPoint.c] != Stone::Empty) return;

    Vector2 center = BoardToScreen(hoverPoint.r, hoverPoint.c);
    float radius = cellSize * 0.46f;

    Color ghostColor = (turnStone == Stone::Black)
        ? ColorAlpha(blackStoneColor, 0.4f)
        : ColorAlpha(whiteStoneColor, 0.6f);

    DrawCircleV(center, radius, ghostColor);
}

void Board::DrawTerritory() const {
    Stone territoryMap[BOARD_SIZE][BOARD_SIZE];
    int blackTerritory = 0;
    int whiteTerritory = 0;
    getTerritory(grid, territoryMap, blackTerritory, whiteTerritory);

    float markSize = cellSize * 0.28f;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (territoryMap[r][c] == Stone::Empty) continue;

            Vector2 center = BoardToScreen(r, c);
            Color markColor = (territoryMap[r][c] == Stone::Black)
                ? ColorAlpha(blackStoneColor, 0.7f)
                : ColorAlpha(whiteStoneColor, 0.85f);

            DrawRectangle(center.x - markSize * 0.5f, center.y - markSize * 0.5f, markSize, markSize, markColor);
        }
    }
}
