#include <bits/stdc++.h>
#include "Board.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
using namespace std;
Board::Board(){
    hasLastMove=false;
    promotionPending=false;
promotionRow=-1;
promotionCol=-1;
    for(int row=0;row<8;row++)
        for(int col=0;col<8;col++)
            squares[row][col]=nullptr;
    squares[0][0]=new Rook(Black);
    squares[0][1]=new Knight(Black);
    squares[0][2]=new Bishop(Black);
    squares[0][3]=new Queen(Black);
    squares[0][4]=new King(Black);
    squares[0][5]=new Bishop(Black);
    squares[0][6]=new Knight(Black);
    squares[0][7]=new Rook(Black);
    for(int col=0;col<8;col++)
        squares[1][col]=new Pawn(Black);
    for(int col=0;col<8;col++)
        squares[6][col]=new Pawn(White);
    squares[7][0]=new Rook(White);
    squares[7][1]=new Knight(White);
    squares[7][2]=new Bishop(White);
    squares[7][3]=new Queen(White);
    squares[7][4]=new King(White);
    squares[7][5]=new Bishop(White);
    squares[7][6]=new Knight(White);
    squares[7][7]=new Rook(White);
}
Board::Board(const Board& other){
    hasLastMove=other.hasLastMove;
    lastMove=other.lastMove;
        promotionPending=other.promotionPending;
    promotionRow=other.promotionRow;
    promotionCol=other.promotionCol;
    for(int row=0;row<8;row++){

        for(int col=0;col<8;col++){

             if(other.squares[row][col])
                squares[row][col]=other.squares[row][col]->clone();
            else
                squares[row][col]=nullptr;
            
        }
    }
}
Board::~Board(){
    for(int row=0;row<8;row++)
        for(int col=0;col<8;col++)
            delete squares[row][col];
}
Piece* Board::getPiece(int row,int col) const{
    return squares[row][col];
}
void Board::printBoard() const{
    for(int row=0;row<8;row++){
        for(int col=0;col<8;col++){
            if(squares[row][col])
                cout<<squares[row][col]->getSymbol()<<" ";
            else
                cout<<". ";
        }   cout<<endl;
  } 
}
bool Board::isInside(int row,int col) const{
    return row>=0 && row<8 && col>=0 && col<8;
}
bool Board::isEmpty(int row,int col) const{
    return squares[row][col]==nullptr;
}
void Board::movePiece(const Move& move){
    Piece* movingPiece=squares[move.fromRow][move.fromCol];
    Piece* piece=squares[move.fromRow][move.fromCol];
    if(auto king=dynamic_cast<King*>(piece))
    king->setHasMoved(true);
    if(auto rook=dynamic_cast<Rook*>(piece))
    rook->setHasMoved(true);

    //pawn enpassant correctin
    if(dynamic_cast<Pawn*>(movingPiece) &&move.fromCol!=move.toCol &&squares[move.toRow][move.toCol]==nullptr)
    {
        int capturedRow=movingPiece->getColor()==White? move.toRow+1: move.toRow-1;
        delete squares[capturedRow][move.toCol];
        squares[capturedRow][move.toCol]=nullptr;
    }
    delete squares[move.toRow][move.toCol];
    squares[move.toRow][move.toCol]=squares[move.fromRow][move.fromCol];
    squares[move.fromRow][move.fromCol]=nullptr;
    if(move.fromRow==7 &&move.fromCol==4 &&move.toRow==7 &&move.toCol==6)
        {
            squares[7][5]=squares[7][7];
            squares[7][7]=nullptr;
        }
        if(move.fromRow==7 &&move.fromCol==4 &&
        move.toRow==7 &&move.toCol==2)
        {
            squares[7][3]=squares[7][0];
            squares[7][0]=nullptr;
        }
        if(move.fromRow==0 && move.fromCol==4 && move.toRow==0 &&move.toCol==6)
        {
            squares[0][5]=squares[0][7];
            squares[0][7]=nullptr;
        }
        if(move.fromRow==0 &&move.fromCol==4 &&move.toRow==0 &&move.toCol==2)
        {
            squares[0][3]=squares[0][0];
            squares[0][0]=nullptr;
        }
    checkPromotion(move.toRow,move.toCol);//checks and promotes both at thse same time
    lastMove=move;
    hasLastMove=true;
}
bool Board::isKingInCheck(Color color) const{
    int kingRow=-1;
    int kingCol=-1;
    char kingSymbol=
        color==White ? 'K' : 'k';
    for(int row=0;row<8;row++){
        for(int col=0;col<8;col++){
            if(squares[row][col] &&squares[row][col]->getSymbol()==kingSymbol){
                kingRow=row;
                kingCol=col;
            }
        }
    }
    Color enemy=  color==White ? Black : White;
    for(int row=0;row<8;row++){
        for(int col=0;col<8;col++){
            Piece* piece=squares[row][col];
            if(piece==nullptr)
                continue;
            if(piece->getColor()!=enemy)
                continue;
            vector<Move> moves=piece->getLegalMoves(*this,row,col);
            for(auto move:moves){
             if(move.toRow==kingRow &&
                   move.toCol==kingCol)
                    return true;
            }
        }
    }
    return false;
}
vector<Move> Board::getAllLegalMoves(Color color) const{
    vector<Move> legalMoves;
    for(int row=0;row<8;row++){
        for(int col=0;col<8;col++){
            Piece* piece=squares[row][col];
            if(piece==nullptr)
                continue;
            if(piece->getColor()!=color)
                continue;
            vector<Move> moves=
                piece->getLegalMoves(*this,row,col);
            for(auto move:moves){
                Board temp(*this);
                temp.movePiece(move);
                if(!temp.isKingInCheck(color))
                    legalMoves.push_back(move);
            }
        }
    }
    return legalMoves;
}
void Board::addCastlingMoves(vector<Move>& moves,int row,int col) const{
    King* king=dynamic_cast<King*>(squares[row][col]);
    if(!king)
        return;
    if(king->getHasMoved())
        return;
    Color enemy=king->getColor()==White? Black: White;
    Rook* kingSide=
        dynamic_cast<Rook*>(squares[row][7]);
    if(kingSide &&kingSide->getColor()==king->getColor() && !kingSide->getHasMoved() && isEmpty(row,5) &&  isEmpty(row,6) &&!isSquareAttacked(row,4,enemy) &&!isSquareAttacked(row,5,enemy) &&!isSquareAttacked(row,6,enemy))
    {
        moves.push_back(Move(row,col,row,6));
    }
    Rook* queenSide=
        dynamic_cast<Rook*>(squares[row][0]);
    if(queenSide &&queenSide->getColor()==king->getColor() &&!queenSide->getHasMoved() && isEmpty(row,1) && isEmpty(row,2) &&isEmpty(row,3) &&!isSquareAttacked(row,4,enemy) &&!isSquareAttacked(row,3,enemy) &&!isSquareAttacked(row,2,enemy))
    {
        moves.push_back(Move(row,col,row,2));
    }
}
bool Board::isSquareAttacked(int row,int col,Color attackerColor) const{
    for(int r=0;r<8;r++){
        for(int c=0;c<8;c++){
            Piece* piece=squares[r][c];
            if(piece==nullptr)
                continue;
            if(piece->getColor()!=attackerColor)
                continue;
            if(dynamic_cast<King*>(piece))
                continue;
            vector<Move> moves=piece->getLegalMoves(*this,r,c);
            for(auto move:moves){
                if(move.toRow==row &&move.toCol==col)
                    return true;
            }
        }
    }
    return false;
}
bool Board::isPromotionPending() const{
    return promotionPending;
}

int Board::getPromotionRow() const{
    return promotionRow;
}

int Board::getPromotionCol() const{
    return promotionCol;
}
void Board::checkPromotion(int row,int col){
    Piece* piece=squares[row][col];
    if(piece==nullptr)
        return;
    char symbol=piece->getSymbol();
    if(symbol=='P' && row==0){
        promotionPending=true;
        promotionRow=row;
        promotionCol=col;
    }
    if(symbol=='p' && row==7){
        promotionPending=true;
        promotionRow=row;
        promotionCol=col;
    }
}
void Board::promoteTo(char choice){
    Piece* piece=squares[promotionRow][promotionCol];
    Color color=piece->getColor();
    delete piece;
    switch(choice){
        case 'R':
            squares[promotionRow][promotionCol]=new Rook(color);
            break;
        case 'B':
            squares[promotionRow][promotionCol]=new Bishop(color);
            break;
        case 'N':
            squares[promotionRow][promotionCol]=new Knight(color);
            break;
        default:
            squares[promotionRow][promotionCol]=new Queen(color);
    }
    promotionPending=false;
}