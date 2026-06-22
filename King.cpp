#include "King.h"
#include "Board.h"
vector<Move> King::getLegalMoves(const Board& board,int row,int col) const{
    vector<Move> moves;
    for(int dr=-1;dr<=1;dr++){
        for(int dc=-1;dc<=1;dc++){
            if(dr==0 && dc==0)
            continue;
            int nr=row+dr;
            int nc=col+dc;
            if(!board.isInside(nr,nc))
            continue;
            Piece* piece=board.getPiece(nr,nc);
            if(piece==nullptr || piece->getColor()!=pieceColor)
            moves.push_back(Move(row,col,nr,nc));
        }
    }
    board.addCastlingMoves(moves,row,col);
    return moves;
}