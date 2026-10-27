#include <bits/stdc++.h>

#include "board.hpp"

#define FOR(i, a, b) for(int i = (a); i <= (b); i++)

Board::Board() : currentTurn(1) {
    reset();
}

void Board::setTurn(int b){currentTurn = b;} 
bool Board::init(int r, int c) const{return (0 <= r && r < BOARD_SIZE) && (0 <= c && c < BOARD_SIZE);}

void Board::reset(){
    FOR(r, 0, BOARD_SIZE - 1){
        FOR(c, 0, BOARD_SIZE - 1){
            grid[r][c] = Stone::Empty;
        }
    }
}

Stone Board::GetStone(int r, int c) const{
    if(!init(r, c)) return Stone::Empty;
    return grid[r][c];
}

void Board::SetStone(int r, int c, Stone p){
    if(init(r, c)){
        grid[r][c] = p;
    }
}

void Board::applyMove(BoardPoint t){
    int r = t.r, c = t.c;
    if(init(r, c) && GetStone(r, c) == Stone::Empty){
        SetStone(r, c, static_cast<Stone>(currentTurn));
        currentTurn = 3 - currentTurn;
    }
}