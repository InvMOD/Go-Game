#pragma once

#include <algorithm>
#include "../lib/raylib.h"

inline Rectangle GetPlayButton(int screenWidth = 696, int screenHeight = 696) {
    float btnWidth = 200.0f;
    float btnHeight = 56.0f;
    float panelH = std::min(330.0f, static_cast<float>(screenHeight) - 40.0f);
    float panelY = (static_cast<float>(screenHeight) - panelH) * 0.5f;
    float centerX = (static_cast<float>(screenWidth) - btnWidth) * 0.5f;
    float centerY = panelY + panelH * 0.62f;
    return {centerX, centerY, btnWidth, btnHeight};
}

inline bool IsPlayButtonClicked(Vector2 mouse, Rectangle button) {
    return CheckCollisionPointRec(mouse, button);
}

inline void DrawMenu(Vector2 mousePosition, int screenWidth = 696, int screenHeight = 696) {
    const Rectangle playButton = GetPlayButton(screenWidth, screenHeight);
    const bool isHovered = CheckCollisionPointRec(mousePosition, playButton);

    SetMouseCursor(isHovered ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

    // Deep warm charcoal background (harmonizes with dark board line theme)
    ClearBackground({32, 28, 25, 255});

    // Board theme colors matching Board/board.hpp
    const Color woodColor = {220, 179, 92, 255};
    const Color lineDarkColor = {50, 35, 20, 255};
    const Color blackStoneColor = {25, 25, 25, 255};
    const Color whiteStoneColor = {245, 245, 245, 255};

    // Central wooden board panel
    float panelW = std::min(420.0f, static_cast<float>(screenWidth) - 40.0f);
    float panelH = std::min(330.0f, static_cast<float>(screenHeight) - 40.0f);
    float panelX = (static_cast<float>(screenWidth) - panelW) * 0.5f;
    float panelY = (static_cast<float>(screenHeight) - panelH) * 0.5f;
    Rectangle boardRect = {panelX, panelY, panelW, panelH};

    // Board drop shadow
    DrawRectangleRec({boardRect.x + 6.0f, boardRect.y + 6.0f, boardRect.width, boardRect.height}, ColorAlpha(BLACK, 0.35f));

    // Wooden board base
    DrawRectangleRec(boardRect, woodColor);
    DrawRectangleLinesEx(boardRect, 3.0f, lineDarkColor);

    // Decorative inner grid lines on the wooden board
    float gridMarginX = 35.0f;
    float gridMarginY = 30.0f;
    float gridW = panelW - 2.0f * gridMarginX;
    float gridH = panelH - 2.0f * gridMarginY;
    int gridCols = 6;
    int gridRows = 5;
    float stepX = gridW / (gridCols - 1);
    float stepY = gridH / (gridRows - 1);
    Color faintLineColor = ColorAlpha(lineDarkColor, 0.22f);

    for (int r = 0; r < gridRows; ++r) {
        float y = panelY + gridMarginY + r * stepY;
        DrawLineEx({panelX + gridMarginX, y}, {panelX + panelW - gridMarginX, y}, 1.0f, faintLineColor);
    }
    for (int c = 0; c < gridCols; ++c) {
        float x = panelX + gridMarginX + c * stepX;
        DrawLineEx({x, panelY + gridMarginY}, {x, panelY + panelH - gridMarginY}, 1.0f, faintLineColor);
    }

    // Star points on the decorative board
    Color starPointColor = ColorAlpha(lineDarkColor, 0.45f);
    DrawCircleV({panelX + gridMarginX + 1 * stepX, panelY + gridMarginY + 1 * stepY}, 2.5f, starPointColor);
    DrawCircleV({panelX + gridMarginX + 4 * stepX, panelY + gridMarginY + 1 * stepY}, 2.5f, starPointColor);
    DrawCircleV({panelX + gridMarginX + 1 * stepX, panelY + gridMarginY + 3 * stepY}, 2.5f, starPointColor);
    DrawCircleV({panelX + gridMarginX + 4 * stepX, panelY + gridMarginY + 3 * stepY}, 2.5f, starPointColor);

    // Title: GO GAME
    const char* title = "GO GAME";
    int fontSize = 48;
    int titleWidth = MeasureText(title, fontSize);
    int titleX = (screenWidth - titleWidth) / 2;
    int titleY = static_cast<int>(panelY + panelH * 0.22f);

    // Title shadow and text using board line color
    DrawText(title, titleX + 2, titleY + 2, fontSize, ColorAlpha(woodColor, 0.4f));
    DrawText(title, titleX, titleY, fontSize, lineDarkColor);

    // Decorative Black & White stones flanking the title
    float stoneRadius = 18.0f;
    float stoneCenterY = static_cast<float>(titleY) + fontSize * 0.5f;

    // Black Stone (left)
    Vector2 blackPos = {static_cast<float>(titleX - 38), stoneCenterY};
    DrawCircle(static_cast<int>(blackPos.x + 2.0f), static_cast<int>(blackPos.y + 2.0f), stoneRadius, ColorAlpha(BLACK, 0.30f));
    DrawCircleV(blackPos, stoneRadius, blackStoneColor);
    DrawCircleLines(static_cast<int>(blackPos.x), static_cast<int>(blackPos.y), stoneRadius, {60, 60, 60, 255});
    DrawCircleV({blackPos.x - 5.0f, blackPos.y - 5.0f}, 3.0f, ColorAlpha(WHITE, 0.15f));

    // White Stone (right)
    Vector2 whitePos = {static_cast<float>(titleX + titleWidth + 38), stoneCenterY};
    DrawCircle(static_cast<int>(whitePos.x + 2.0f), static_cast<int>(whitePos.y + 2.0f), stoneRadius, ColorAlpha(BLACK, 0.25f));
    DrawCircleV(whitePos, stoneRadius, whiteStoneColor);
    DrawCircleLines(static_cast<int>(whitePos.x), static_cast<int>(whitePos.y), stoneRadius, {200, 200, 200, 255});
    DrawCircleV({whitePos.x - 5.0f, whitePos.y - 5.0f}, 4.0f, ColorAlpha(WHITE, 0.65f));

    // Play Button (styled like a polished dark wood / lacquer piece)
    Rectangle btnShadow = {playButton.x + 3.0f, playButton.y + 3.0f, playButton.width, playButton.height};
    DrawRectangleRounded(btnShadow, 0.30f, 8, ColorAlpha(BLACK, 0.30f));

    Color btnColor = isHovered ? Color{65, 45, 30, 255} : Color{45, 32, 22, 255};
    Color btnBorderColor = isHovered ? Color{250, 215, 140, 255} : Color{180, 140, 70, 255};

    DrawRectangleRounded(playButton, 0.30f, 8, btnColor);
    DrawRectangleRoundedLinesEx(playButton, 0.30f, 8, 2.0f, btnBorderColor);

    // Text: PLAY (exact original text, no extra letters)
    const char* playText = "PLAY";
    int playFontSize = 28;
    int playTextWidth = MeasureText(playText, playFontSize);
    int playX = static_cast<int>(playButton.x + (playButton.width - playTextWidth) * 0.5f);
    int playY = static_cast<int>(playButton.y + (playButton.height - playFontSize) * 0.5f);

    DrawText(playText, playX + 1, playY + 1, playFontSize, ColorAlpha(BLACK, 0.5f));
    DrawText(playText, playX, playY, playFontSize, isHovered ? WHITE : Color{245, 240, 230, 255});
}
