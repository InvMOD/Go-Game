#pragma once

#include "../lib/raylib.h"

inline Rectangle GetPlayButton(int screenWidth = 696, int screenHeight = 696) {
    float btnWidth = 200.0f;
    float btnHeight = 64.0f;
    return {(screenWidth - btnWidth) * 0.5f, (screenHeight - btnHeight) * 0.5f + 30.0f, btnWidth, btnHeight};
}

inline bool IsPlayButtonClicked(Vector2 mouse, Rectangle button) {
    return CheckCollisionPointRec(mouse, button);
}

inline void DrawMenu(Vector2 mousePosition, int screenWidth = 696, int screenHeight = 696) {
    const Rectangle playButton = GetPlayButton(screenWidth, screenHeight);
    const bool isHovered = CheckCollisionPointRec(mousePosition, playButton);

    ClearBackground({24, 28, 40, 255});

    const char* title = "GO GAME";
    int fontSize = 48;
    int titleWidth = MeasureText(title, fontSize);
    DrawText(title, (screenWidth - titleWidth) / 2, (int)(playButton.y - 110), fontSize, RAYWHITE);

    DrawRectangleRec(playButton, isHovered ? GREEN : DARKGREEN);

    const char* playText = "PLAY";
    int playFontSize = 28;
    int playTextWidth = MeasureText(playText, playFontSize);
    DrawText(playText, 
             (int)(playButton.x + (playButton.width - playTextWidth) * 0.5f), 
             (int)(playButton.y + (playButton.height - playFontSize) * 0.5f), 
             playFontSize, RAYWHITE);
}
