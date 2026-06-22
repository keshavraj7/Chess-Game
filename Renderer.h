#pragma once

#include <SFML/Graphics.hpp>
#include <bits\stdc++.h>
#include "Board.h"
#include "Move.h"
#include "Piece.h"
#include "AI.h"
using namespace std;
class Renderer{
private:
    sf::RenderWindow window;
    sf::Font font;
    unordered_map<char,sf::Texture> textures;
    int selectedRow;
    int selectedCol;
    vector<Move> highlightedMoves;
    void loadTextures();
    bool gameOver;
    string gameOverText;
    bool promotionMenuVisible;
    AI ai;
bool vsComputer;
Color humanColor;
Color aiColor;
Move lastPlayedMove;
bool hasLastPlayedMove;
public:
    Renderer();
    bool isOpen() const;
    void processEvents();
    void draw(const Board& board);
    bool handleClick(Board& board,Color& currentTurn,int mouseX,int mouseY);
    sf::RenderWindow& getWindow();
    void setGameMode(int mode);
};