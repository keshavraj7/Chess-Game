#pragma once

#include "Piece.h"

class Knight:public Piece{
public:
    Knight(Color color):Piece(color){}

    vector<Move> getLegalMoves(const Board& board,int row,int col) const override;

    char getSymbol() const override{
        return pieceColor==White?'N':'n';
    }
    Piece* clone() const override{
    return new Knight(*this);
}
};