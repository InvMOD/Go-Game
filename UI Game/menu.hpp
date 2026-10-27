#pragma once

#include "../lib/raylib.h"

inline Rectangle GetPlayButton() {
    return {248, 330, 200, 64};
}

inline bool IsPlayButtonClicked(Vector2 mouse, Rectangle button) {
    return CheckCollisionPointRec(mouse, button);
}

inline void DrawMenu(Vector2 mousePosition) {
    const Rectangle playButton = GetPlayButton();
    const bool isHovered = CheckCollisionPointRec(mousePosition, playButton);

    ClearBackground({24, 28, 40, 255});
    DrawText("GO GAME", 232, 190, 48, RAYWHITE);
    DrawRectangleRec(playButton, isHovered ? GREEN : DARKGREEN);
    DrawText("PLAY", 310, 348, 28, RAYWHITE);
}
