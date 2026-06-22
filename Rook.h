#pragma once
#include "Piece.h"
class Rook:public Piece{
private:
    bool hasMoved;
public:
    Rook(Color color):Piece(color),hasMoved(false){}
    bool getHasMoved() const{return hasMoved;}
    void setHasMoved(bool value){hasMoved=value;}
    vector<Move> getLegalMoves(const Board& board,int row,int col) const override;
    char getSymbol() const override{
        return pieceColor==White?'R':'r';
    }
    Piece* clone() const override{
        return new Rook(*this);
    }
};