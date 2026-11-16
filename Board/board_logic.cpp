#include "board.hpp"
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
    lastMove = {-1, -1};
    history.clear();
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

void Board::getGroupAndLiberties(const Stone b[BOARD_SIZE][BOARD_SIZE], int startR, int startC,
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

void Board::getTerritory(const Stone b[BOARD_SIZE][BOARD_SIZE], Stone territoryMap[BOARD_SIZE][BOARD_SIZE], int& blackTerritory, int& whiteTerritory) {
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

    // suicide
    std::vector<BoardPoint> myGroup;
    std::set<std::pair<int, int>> myLibs;
    getGroupAndLiberties(tempGrid, t.r, t.c, myGroup, myLibs);

    if (myLibs.empty()) return;

    // KO' rule
    if (!history.empty()) {
        bool isKo = true;
        const auto& prevGrid = history.back().grid;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (tempGrid[i][j] != prevGrid[i][j]) {
                    isKo = false;
                    break;
                }
            }
            if (!isKo) break;
        }
        if (isKo) return; 
    }

    // save state for redo
    BoardState state;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            state.grid[i][j] = grid[i][j];
        }
    }
    state.currentTurn = currentTurn;
    state.blackCaptures = blackCaptures;
    state.whiteCaptures = whiteCaptures;
    state.lastMove = lastMove;
    history.push_back(state);

    // new board
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

    lastMove = t;
    currentTurn = (currentTurn == 1) ? 2 : 1;
}

bool Board::undo() {
    if (history.empty()) return false;

    BoardState prev = history.back();
    history.pop_back();

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            grid[i][j] = prev.grid[i][j];
        }
    }
    currentTurn = prev.currentTurn;
    blackCaptures = prev.blackCaptures;
    whiteCaptures = prev.whiteCaptures;
    lastMove = prev.lastMove;
    return true;
}

bool Board::canUndo() const {
    return !history.empty();
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
