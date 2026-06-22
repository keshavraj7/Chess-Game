#pragma once
#include "Board.h"
class Game{
private:
    Board board;
    Color currentTurn;
public:
    Game();
    void play();
    bool isMoveLegal(int fr,int fc,int tr,int tc);
};