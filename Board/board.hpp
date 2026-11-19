#pragma once
#include <vector>
#include <set>
#include <utility>
#include <cstdint>
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

struct BoardScore {
    int blackTerritory;
    int whiteTerritory;
    int blackCaptures;
    int whiteCaptures;
    int blackTotal;
    int whiteTotal;
}; 

struct BoardState {
    Stone grid[BOARD_SIZE][BOARD_SIZE];
    int currentTurn;
    int blackCaptures;
    int whiteCaptures;
    BoardPoint lastMove;
    uint64_t hash;
};

class Board { 
public: 
    float cellSize;
    float offsetX;
    float offsetY;
    float gridSpan;
    float boardPadding;

    Stone grid[BOARD_SIZE][BOARD_SIZE];
    int currentTurn;
    int blackCaptures;
    int whiteCaptures;
    BoardPoint lastMove;
    uint64_t currentHash;
    std::vector<BoardState> history;

    Board();
    ~Board();
    void reset();
    void setTurn(int b);
    bool applyMove(BoardPoint t);
    bool undo();
    bool canUndo() const;
    void updateLayout(int screenWidth, int screenHeight);

    bool init(int r, int c) const;
    Stone GetStone(int r, int c) const;
    void SetStone(int r, int c, Stone stone);

    Vector2 BoardToScreen(int r, int c) const;
    BoardPoint ScreenToBoard(Vector2 mousePos) const;

    BoardScore calculateScore() const;

    bool isSuperko(uint64_t hash) const;
    static uint64_t computeHash(const Stone b[BOARD_SIZE][BOARD_SIZE]);
    static void initZobrist();

    void DrawBase() const; 
    void DrawStones() const;
    void DrawGhost(BoardPoint hoverPoint, Stone currentTurn) const;
    void DrawTerritory() const;

    void initAudio();
    void playStoneSound();
    void closeAudio();

    static void getGroupAndLiberties(const Stone b[BOARD_SIZE][BOARD_SIZE], int startR, int startC,
                                     std::vector<BoardPoint>& group, std::set<std::pair<int, int>>& liberties);
    static void getTerritory(const Stone b[BOARD_SIZE][BOARD_SIZE], Stone territoryMap[BOARD_SIZE][BOARD_SIZE],
                             int& blackTerritory, int& whiteTerritory);

private: 
    Color woodColor = {220, 179, 92, 255}; // RGB Color
    Color lineDarkColor = {50, 35, 20, 255}; 
    Color blackStoneColor = {25, 25, 25, 255};
    Color whiteStoneColor = {245, 245, 245, 255};

    std::vector<Sound> stoneSounds;
    bool audioLoaded = false;
};
