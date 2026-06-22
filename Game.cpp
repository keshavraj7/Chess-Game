#include <iostream>
#include "Game.h"

using namespace std;

Game::Game(){
    currentTurn=White;
}

void Game::play(){
    while(true){
        board.printBoard();
        int fr,fc,tr,tc;
        cout<<"\nMove: ";
        cin>>fr>>fc>>tr>>tc;
        Piece* piece=board.getPiece(fr,fc);
        if(piece==nullptr){
            cout<<"No piece selected\n";
            continue;
        }
        if(piece->getColor()!=currentTurn){
            cout<<"Wrong turn\n";
            continue;
        }
        vector<Move> legalMoves= board.getAllLegalMoves(currentTurn);
        bool valid=false;
        for(auto move:legalMoves){
            if(move.fromRow==fr &&move.fromCol==fc &&move.toRow==tr&&move.toCol==tc){
                valid=true;
                break;
            }
        }
        if(!valid){
            cout<<"Illegal move\n";
            continue;
        }
        board.movePiece(Move(fr,fc,tr,tc));
        Color nextTurn=
            (currentTurn==White)? Black
            : White;
            if(board.isKingInCheck(nextTurn)&&board.getAllLegalMoves(nextTurn).empty()){
                cout<<"CHECKMATE!\n";
                board.printBoard();
                break;
            }
            else if(board.isKingInCheck(nextTurn))
            cout<<"CHECK!\n";

            if(!board.isKingInCheck(nextTurn)&&board.getAllLegalMoves(nextTurn).empty()){
                cout<<"STALEMATE!\n";
                 board.printBoard();
                break;
            }
            currentTurn=nextTurn;
    }
}
bool Game::isMoveLegal(int fr,int fc,int tr,int tc){
    Piece* piece=board.getPiece(fr,fc);
    if(piece==nullptr)
    return false;
    if(piece->getColor()!=currentTurn)
    return false;
    vector<Move> moves=
    piece->getLegalMoves(board,fr,fc);
    for(auto move:moves){
        if(move.toRow==tr &&move.toCol==tc)
        return true;
    }
    return false;
}