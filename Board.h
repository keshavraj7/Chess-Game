#pragma once

#include "Piece.h"
class Board{
private:
    Piece* squares[8][8];
    Move lastMove;
    bool hasLastMove;
    bool promotionPending;
    int promotionRow;
    int promotionCol;
public:
    Board();
   Board(const Board& other);
    ~Board();
    Piece* getPiece(int row,int col) const;

    void printBoard() const;
bool isInside(int row,int col) const;
bool isEmpty(int row,int col) const;
void movePiece(const Move& move);
    bool isKingInCheck(Color color) const;
    vector<Move> getAllLegalMoves(Color color) const;
    void addCastlingMoves(vector<Move>& moves,int row,int col) const;
    bool isSquareAttacked(int row,int col,Color attackerColor) const;//for castling
    Move getLastMove() const{return lastMove;}
    bool getHasLastMove() const{return hasLastMove;}
    bool isPromotionPending() const;
    int getPromotionRow() const;
    int getPromotionCol() const;
    void checkPromotion(int row,int col);
    void promoteTo(char choice);
};