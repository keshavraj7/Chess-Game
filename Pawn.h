#pragma once

#include "Piece.h"

class Pawn:public Piece{
public:
    Pawn(Color color):Piece(color){}

    vector<Move> getLegalMoves(const Board& board,int row,int col) const override;

    char getSymbol() const override{
        return pieceColor==White?'P':'p';
    }
    Piece* clone() const override{
    return new Pawn(*this);
}
};