#include "Pawn.h"
#include "Board.h"
#include <bits/stdc++.h>
vector<Move> Pawn::getLegalMoves(const Board& board,int row,int col) const{
    vector<Move> moves;
    int dir=(pieceColor==White?-1:1);
    int nr=row+dir;
    if(board.isInside(nr,col) && board.isEmpty(nr,col))
        moves.push_back(Move(row,col,nr,col));
    if(pieceColor==White && row==6 &&board.isEmpty(5,col) &&board.isEmpty(4,col))
        moves.push_back(Move(row,col,4,col));
    if(pieceColor==Black && row==1&&board.isEmpty(2,col) && board.isEmpty(3,col))
        moves.push_back(Move(row,col,3,col));
      for(int dc:{-1,1}){
          int nc=col+dc;
          if(!board.isInside(nr,nc))
              continue;
          Piece* piece=board.getPiece(nr,nc);
          if(piece && piece->getColor()!=pieceColor)
              moves.push_back(Move(row,col,nr,nc));
      }
        if(board.getHasLastMove()){
            Move last=board.getLastMove();
            int enemyStartRow=pieceColor==White ? 1 :6;
            int enemyEndRow=pieceColor==White ? 3:4;
            if(last.fromRow==enemyStartRow &&last.toRow==enemyEndRow)
            {
                Piece* enemy= board.getPiece(last.toRow,last.toCol);
                if(enemy &&dynamic_cast<Pawn*>(enemy) &&enemy->getColor()!=pieceColor)
                {
                    if(row==enemyEndRow &&abs(col-last.toCol)==1)
                    {
                        moves.push_back(
                            Move(row,col,row+dir, last.toCol)
                        );
                    }
                }
            }
        }
    return moves;
}