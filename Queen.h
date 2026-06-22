#pragma once

#include "Piece.h"

class Queen:public Piece{
public:
    Queen(Color color):Piece(color){}

    vector<Move> getLegalMoves(const Board& board,int row,int col) const override;

    char getSymbol() const override{
        return pieceColor==White?'Q':'q';
    }
    Piece* clone() const override{
    return new Queen(*this);
}
};