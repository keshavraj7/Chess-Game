#pragma once

#include "Piece.h"

class King:public Piece{
    private:
    bool hasMoved;
public:
   King(Color color):Piece(color),hasMoved(false){}
     bool getHasMoved() const{return hasMoved;}
    void setHasMoved(bool value){hasMoved=value;}
    vector<Move> getLegalMoves(const Board& board,int row,int col) const override;

    char getSymbol() const override{
        return pieceColor==White?'K':'k';
    }
    Piece* clone() const override{
        return new King(*this);
    }
};