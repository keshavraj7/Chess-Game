#include "AI.h"
#include <bits/stdc++.h>

int AI::evaluate(const Board& board){
    int score=0;
    for(int row=0;row<8;row++){
        for(int col=0;col<8;col++){
            Piece* piece=board.getPiece(row,col);
            if(piece==nullptr)
                continue;
            int value=0;
            switch(tolower(piece->getSymbol())){
                case 'p':
                    value=100;
                    break;
                case 'n':
                    value=320;
                    break;
                case 'b':
                    value=330;
                    break;
                case 'r':
                    value=500;
                    break;
                case 'q':
                    value=900;
                    break;
                case 'k':
                    value=20000;
                    break;
            }
            if(piece->getColor()==White)
                score+=value;
            else
                score-=value;
        }
    }
    return score;
}
int AI::alphaBeta(Board board,int depth,int alpha,int beta,Color currentTurn
){
    if(depth==0)
        return evaluate(board);
    vector<Move> moves=
        board.getAllLegalMoves(currentTurn);
    if(moves.empty()){
        if(board.isKingInCheck(currentTurn))
            return currentTurn==White? -100000: 100000;
        return 0;
    }
    if(currentTurn==White){
        int best=INT_MIN;
        for(auto move:moves){
            Board temp(board);
            temp.movePiece(move);
            if(temp.isPromotionPending())
                temp.promoteTo('Q');
            best=max(
                best,
                alphaBeta(temp,depth-1,alpha,beta,Black)
            );
            alpha=max(alpha,best);
            if(beta<=alpha)
                break;
        }
        return best;
    }
    int best=INT_MAX;
    for(auto move:moves){
        Board temp(board);
        temp.movePiece(move);
        if(temp.isPromotionPending())
            temp.promoteTo('Q');
        best=min(best, alphaBeta(temp, depth-1,alpha,beta,White )
        );
        beta=min(beta,best);
        if(beta<=alpha)
            break;
    }
    return best;
}
Move AI::findBestMove(Board board,Color aiColor,  int depth
){
    vector<Move> moves=
        board.getAllLegalMoves(aiColor);
    Move bestMove=moves[0];
    if(aiColor==White){
        int bestScore=INT_MIN;
        for(auto move:moves){
            Board temp(board);
            temp.movePiece(move);
            if(temp.isPromotionPending())
                temp.promoteTo('Q');
            int score=alphaBeta(temp,depth-1,INT_MIN,INT_MAX,Black);
            if(score>bestScore){
                bestScore=score;
                bestMove=move;
            }
        }
        return bestMove;
    }
    int bestScore=INT_MAX;
    for(auto move:moves){
        Board temp(board);
        temp.movePiece(move);
        if(temp.isPromotionPending())
            temp.promoteTo('Q');
        int score= alphaBeta(temp,depth-1,INT_MIN,INT_MAX,White);
        if(score<bestScore){
            bestScore=score;
           bestMove=move;
        }
    }
    return bestMove;
}