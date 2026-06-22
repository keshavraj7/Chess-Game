#pragma once

#include "Piece.h"

class Bishop:public Piece{
public:
    Bishop(Color color):Piece(color){}
    vector<Move> getLegalMoves(const Board& board,int row,int col) const override;
    char getSymbol() const override{
        return pieceColor==White?'B':'b';
    }
    Piece* clone() const override{
    return new Bishop(*this);
}
};