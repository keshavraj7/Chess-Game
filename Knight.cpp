#include "Knight.h"
#include "Board.h"
vector<Move> Knight::getLegalMoves(const Board& board,int row,int col) const{
    vector<Move> moves;
    int dr[]={2,2,-2,-2,1,1,-1,-1};
    int dc[]={1,-1,1,-1,2,-2,2,-2};
    for(int i=0;i<8;i++){
        int nr=row+dr[i];
        int nc=col+dc[i];
        if(!board.isInside(nr,nc))
            continue;
        Piece* piece=board.getPiece(nr,nc);
        if(piece==nullptr || piece->getColor()!=pieceColor)
            moves.push_back(Move(row,col,nr,nc));
    }
    return moves;
}