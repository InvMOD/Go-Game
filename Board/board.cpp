#include "Board.hpp"
#include <algorithm>
#include <cmath>
#include <queue>
#include <set>
#include <vector>

Board::Board() {
    reset();
    updateLayout(696, 696);
}

void Board::reset() {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            grid[r][c] = Stone::Empty;
        }
    }
    currentTurn = 1;
    blackCaptures = 0;
    whiteCaptures = 0;
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

static void getTerritory(const Stone b[BOARD_SIZE][BOARD_SIZE], Stone territoryMap[BOARD_SIZE][BOARD_SIZE], int& blackTerritory, int& whiteTerritory) {
    blackTerritory = 0;
    whiteTerritory = 0;
    bool visited[BOARD_SIZE][BOARD_SIZE] = {false};

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            territoryMap[r][c] = Stone::Empty;
        }
    }

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (b[r][c] == Stone::Empty && !visited[r][c]) {
                std::vector<BoardPoint> region;
                std::queue<BoardPoint> q;
                q.push({r, c});
                visited[r][c] = true;

                bool touchesBlack = false;
                bool touchesWhite = false;

                while (!q.empty()) {
                    BoardPoint p = q.front();
                    q.pop();
                    region.push_back(p);

                    for (int i = 0; i < 4; ++i) {
                        int nr = p.r + dr[i];
                        int nc = p.c + dc[i];
                        if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) continue;

                        if (b[nr][nc] == Stone::Black) {
                            touchesBlack = true;
                        } else if (b[nr][nc] == Stone::White) {
                            touchesWhite = true;
                        } else if (b[nr][nc] == Stone::Empty && !visited[nr][nc]) {
                            visited[nr][nc] = true;
                            q.push({nr, nc});
                        }
                    }
                }

                Stone owner = Stone::Empty;
                if (touchesBlack && !touchesWhite) {
                    owner = Stone::Black;
                    blackTerritory += static_cast<int>(region.size());
                } else if (touchesWhite && !touchesBlack) {
                    owner = Stone::White;
                    whiteTerritory += static_cast<int>(region.size());
                }

                if (owner != Stone::Empty) {
                    for (const auto& p : region) {
                        territoryMap[p.r][p.c] = owner;
                    }
                }
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

    int capturedCount = 0;
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
                capturedCount += static_cast<int>(oppGroup.size());
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

    if (myStone == Stone::Black) {
        blackCaptures += capturedCount;
    } else {
        whiteCaptures += capturedCount;
    }

    currentTurn = (currentTurn == 1) ? 2 : 1;
}

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

            // Đổ bóng quân cờ
            DrawCircle(center.x + shadowOffset, center.y + shadowOffset, radius, ColorAlpha(BLACK, 0.25f));

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

BoardScore Board::calculateScore() const {
    Stone territoryMap[BOARD_SIZE][BOARD_SIZE];
    int blackTerritory = 0;
    int whiteTerritory = 0;
    getTerritory(grid, territoryMap, blackTerritory, whiteTerritory);

    BoardScore score;
    score.blackTerritory = blackTerritory;
    score.whiteTerritory = whiteTerritory;
    score.blackCaptures = blackCaptures;
    score.whiteCaptures = whiteCaptures;
    score.blackTotal = blackTerritory + blackCaptures;
    score.whiteTotal = whiteTerritory + whiteCaptures;
    return score;
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
