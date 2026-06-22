#include "Renderer.h"
#include "Board.h"

int main(){

    Board board;
Renderer renderer;
renderer.setGameMode(0);
    Color currentTurn=White;
    while(renderer.isOpen()){
        while(const auto event=renderer.getWindow().pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                renderer.getWindow().close();
            if(const auto* mouse=
               event->getIf<sf::Event::MouseButtonPressed>())
            {
                if(renderer.isOpen())  
                renderer.handleClick(board,currentTurn,mouse->position.x,mouse->position.y);
            }
        }
        renderer.draw(board);
    }
    return 0;
}