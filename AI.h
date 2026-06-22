#pragma once

#include "Board.h"

class AI{
public:
    Move findBestMove(Board board,Color aiColor,int depth);

private:
    int alphaBeta(Board board,int depth,int alpha,int beta,Color currentTurn);
    int evaluate(const Board& board);
};