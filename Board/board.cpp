#include "Board.hpp"
#include <cmath>
#include <queue>
#include <set>
#include <vector>

Board::Board() {
    reset();
}

void Board::reset() {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            grid[r][c] = Stone::Empty;
        }
    }
    currentTurn = 1;
}

void Board::setTurn(int b) {
    currentTurn = b;
}

bool Board::init(int r, int c) const {
    return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE;
}

Stone Board::GetStone(int r, int c) const {
    if (!init(r, c)) return Stone::Empty;
    return grid[r][c];
}

void Board::SetStone(int r, int c, Stone stone) {
    if (init(r, c)) {
        grid[r][c] = stone;
    }
}

static void getGroupAndLiberties(const Stone b[BOARD_SIZE][BOARD_SIZE], int startR, int startC,
                                 std::vector<BoardPoint>& group, std::set<std::pair<int, int>>& liberties) {
    Stone color = b[startR][startC];
    bool visited[BOARD_SIZE][BOARD_SIZE] = {false};
    std::queue<BoardPoint> q;

    q.push({startR, startC});
    visited[startR][startC] = true;

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        BoardPoint p = q.front();
        q.pop();
        group.push_back(p);

        for (int i = 0; i < 4; ++i) {
            int nr = p.r + dr[i];
            int nc = p.c + dc[i];
            if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) continue;

            if (b[nr][nc] == Stone::Empty) {
                liberties.insert({nr, nc});
            } else if (b[nr][nc] == color && !visited[nr][nc]) {
                visited[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }
}

void Board::applyMove(BoardPoint t) {
    if (!init(t.r, t.c) || grid[t.r][t.c] != Stone::Empty) return;

    Stone myStone = (currentTurn == 1) ? Stone::Black : Stone::White;
    Stone oppStone = (currentTurn == 1) ? Stone::White : Stone::Black;

    Stone tempGrid[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            tempGrid[i][j] = grid[i][j];
        }
    }

    tempGrid[t.r][t.c] = myStone;

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int nr = t.r + dr[i];
        int nc = t.c + dc[i];
        if (init(nr, nc) && tempGrid[nr][nc] == oppStone) {
            std::vector<BoardPoint> oppGroup;
            std::set<std::pair<int, int>> oppLibs;
            getGroupAndLiberties(tempGrid, nr, nc, oppGroup, oppLibs);

            if (oppLibs.empty()) {
                for (const auto& p : oppGroup) {
                    tempGrid[p.r][p.c] = Stone::Empty;
                }
            }
        }
    }

    std::vector<BoardPoint> myGroup;
    std::set<std::pair<int, int>> myLibs;
    getGroupAndLiberties(tempGrid, t.r, t.c, myGroup, myLibs);

    if (myLibs.empty()) return;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            grid[i][j] = tempGrid[i][j];
        }
    }
    currentTurn = (currentTurn == 1) ? 2 : 1;
}

Vector2 Board::BoardToScreen(int r, int c) const {
    return {
        (float)(margin + c * cellSize),
        (float)(margin + r * cellSize)
    };
}

BoardPoint Board::ScreenToBoard(Vector2 mousePos) const {
    int c = std::round((mousePos.x - margin) / (float)cellSize);
    int r = std::round((mousePos.y - margin) / (float)cellSize);
    return {r, c};
}

void Board::DrawBase() const {
    ClearBackground(woodColor);

    DrawRectangleLines(margin - 12, margin - 12, gridSpan + 24, gridSpan + 24, lineDarkColor);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        int offset = margin + i * cellSize;
        DrawLine(margin, offset, margin + gridSpan, offset, lineDarkColor);
        DrawLine(offset, margin, offset, margin + gridSpan, lineDarkColor);
    }

    const BoardPoint starPoints[] = {
        {2, 2}, {2, 6},
        {4, 4},
        {6, 2}, {6, 6}
    };

    for (const auto& p : starPoints) {
        Vector2 pos = BoardToScreen(p.r, p.c);
        DrawCircleV(pos, 4.0f, lineDarkColor);
    }
}

void Board::DrawStones() const {
    float radius = cellSize * 0.46f;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (grid[r][c] == Stone::Empty) continue;

            Vector2 center = BoardToScreen(r, c);
            DrawCircle(center.x + 2, center.y + 2, radius, ColorAlpha(BLACK, 0.25f));

            if (grid[r][c] == Stone::Black) {
                DrawCircleV(center, radius, blackStoneColor);
                DrawCircleLines(center.x, center.y, radius, {60, 60, 60, 255});
            } else {
                DrawCircleV(center, radius, whiteStoneColor);
                DrawCircleLines(center.x, center.y, radius, {200, 200, 200, 255});
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