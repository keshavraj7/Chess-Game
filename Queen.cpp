#include "Queen.h"
#include "Board.h"
vector<Move> Queen::getLegalMoves(const Board& board,int row,int col) const{
    vector<Move> moves;
    int dr[]={1,-1,0,0,1,1,-1,-1};
    int dc[]={0,0,1,-1,1,-1,1,-1};
    for(int dir=0;dir<8;dir++){
        int nr=row+dr[dir];
        int nc=col+dc[dir];
        while(board.isInside(nr,nc)){
            Piece* piece=board.getPiece(nr,nc);
            if(piece==nullptr){
                moves.push_back(Move(row,col,nr,nc));
            }
            else{
                if(piece->getColor()!=pieceColor)
                    moves.push_back(Move(row,col,nr,nc));
                break;
            }
            nr+=dr[dir];
            nc+=dc[dir];
        }
    }
    return moves;
}