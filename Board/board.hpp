#pragma once
#include <vector>
#include "../lib/raylib.h"

const int BOARD_SIZE = 9;

enum class Stone { 
    Empty = 0, 
    Black = 1,
    White = 2
}; 

struct BoardPoint { 
    int r, c;
}; 

class Board { 
public: 
    const int cellSize = 64;
    const int margin = 60;
    const int gridSpan = (BOARD_SIZE - 1) * cellSize;

    Stone grid[BOARD_SIZE][BOARD_SIZE];
    int currentTurn;

    Board();
    void reset();
    void setTurn(int b);
    void applyMove(BoardPoint t);

    bool init(int r, int c) const;
    Stone GetStone(int r, int c) const;
    void SetStone(int r, int c, Stone stone);

    Vector2 BoardToScreen(int r, int c) const;
    BoardPoint ScreenToBoard(Vector2 mousePos) const;

    void DrawBase() const; 
    void DrawStones() const;
    void DrawGhost(BoardPoint hoverPoint, Stone currentTurn) const;

private: 
    Color woodColor = {220, 179, 92, 255}; // RGB Color
    Color lineDarkColor = {50, 35, 20, 255}; 
    Color blackStoneColor = {25, 25, 25, 255};
    Color whiteStoneColor = {245, 245, 245, 255};
};

