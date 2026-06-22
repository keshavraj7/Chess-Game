#pragma once

#include <vector>
#include "Move.h"
using namespace std;

class Board;

enum Color{White,Black};

class Piece{
protected:
    Color pieceColor;

public:
    Piece(Color color):pieceColor(color){}
    virtual ~Piece()=default;

    Color getColor() const{return pieceColor;}

    virtual vector<Move> getLegalMoves(const Board& board,int row,int col) const=0;
    virtual char getSymbol() const=0;
    virtual Piece* clone() const=0;
};