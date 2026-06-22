#include "Renderer.h"
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
using namespace std;

Renderer::Renderer()
    : window(sf::VideoMode({800, 800}), "Chess", sf::Style::Default | sf::Style::Resize)
{
    selectedRow = -1;
    selectedCol = -1;
    gameOver = false;
    promotionMenuVisible = false;
    vsComputer = false;
    humanColor = White;
    aiColor = Black;
    hasLastPlayedMove=false;
    font.openFromFile("ALGER.TTF");
    loadTextures();
}

bool Renderer::isOpen() const {
    return window.isOpen();
}

sf::RenderWindow& Renderer::getWindow() {
    return window;
}

void Renderer::processEvents() {
    while (const auto event = window.pollEvent()) {
        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            sf::FloatRect visibleArea({0.f, 0.f},
                {static_cast<float>(resized->size.x),
                 static_cast<float>(resized->size.y)});
            window.setView(sf::View(visibleArea));
        }
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Renderer::setGameMode(int mode) {
    int phase = 0;
    while (window.isOpen()) {
        window.clear(sf::Color(30, 30, 30));

        float w = static_cast<float>(window.getSize().x);
        float h = static_cast<float>(window.getSize().y);
        float btnW = w * 0.5f;
        float btnH = h * 0.1f;
        float btnX = (w - btnW) / 2.f;
        unsigned int fontSize = static_cast<unsigned int>(h * 0.04f);

        if (phase == 0) {
            sf::Text title(font);
            title.setString("CHESS");
            title.setCharacterSize(static_cast<unsigned int>(h * 0.09f));
            title.setFillColor(sf::Color::White);
            title.setOutlineColor(sf::Color::Black);
            title.setOutlineThickness(3.f);
            auto tb = title.getLocalBounds();
            title.setPosition({(w - tb.size.x) / 2.f, h * 0.1f});
            window.draw(title);

            string labels[2] = {"2 Players", "vs Computer"};
            float ys[2] = {h * 0.38f, h * 0.55f};

            for (int i = 0; i < 2; i++) {
                sf::RectangleShape box({btnW, btnH});
                box.setPosition({btnX, ys[i]});
                box.setFillColor(sf::Color(70, 130, 180));
                window.draw(box);

                sf::Text label(font);
                label.setString(labels[i]);
                label.setCharacterSize(fontSize);
                label.setFillColor(sf::Color::White);
                auto lb = label.getLocalBounds();
                label.setPosition({btnX + (btnW - lb.size.x) / 2.f,
                                   ys[i] + (btnH - lb.size.y) / 2.f});
                window.draw(label);
            }
        }
        else {
            sf::Text title(font);
            title.setString("Play as:");
            title.setCharacterSize(static_cast<unsigned int>(h * 0.07f));
            title.setFillColor(sf::Color::White);
            auto tb = title.getLocalBounds();
            title.setPosition({(w - tb.size.x) / 2.f, h * 0.2f});
            window.draw(title);

            string labels[2] = {"White", "Black"};
            float ys[2] = {h * 0.42f, h * 0.58f};
            sf::Color colors[2] = {sf::Color(240, 217, 181), sf::Color(80, 80, 80)};

            for (int i = 0; i < 2; i++) {
                sf::RectangleShape box({btnW, btnH});
                box.setPosition({btnX, ys[i]});
                box.setFillColor(colors[i]);
                box.setOutlineColor(sf::Color::White);
                box.setOutlineThickness(2.f);
                window.draw(box);

                sf::Text label(font);
                label.setString(labels[i]);
                label.setCharacterSize(fontSize);
                label.setFillColor(i == 0 ? sf::Color::Black : sf::Color::White);
                auto lb = label.getLocalBounds();
                label.setPosition({btnX + (btnW - lb.size.x) / 2.f,
                                   ys[i] + (btnH - lb.size.y) / 2.f});
                window.draw(label);
            }
        }

        window.display();
        while (const auto event = window.pollEvent()) {
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea({0.f, 0.f},
                    {static_cast<float>(resized->size.x),
                     static_cast<float>(resized->size.y)});
                window.setView(sf::View(visibleArea));
            }
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (click->button != sf::Mouse::Button::Left)
                    continue;

                sf::Vector2f pos = window.mapPixelToCoords(
                    sf::Vector2i(click->position.x, click->position.y));

                float w2 = static_cast<float>(window.getSize().x);
                float h2 = static_cast<float>(window.getSize().y);
                float bW = w2 * 0.5f;
                float bH = h2 * 0.1f;
                float bX = (w2 - bW) / 2.f;

                if (phase == 0) {
                    float ys[2] = {h2 * 0.38f, h2 * 0.55f};
                    if (pos.x >= bX && pos.x <= bX + bW) {
                        if (pos.y >= ys[0] && pos.y <= ys[0] + bH) {
                            vsComputer = false;
                            return;
                        }
                        if (pos.y >= ys[1] && pos.y <= ys[1] + bH) {
                            vsComputer = true;
                            phase = 1;
                        }
                    }
                }
                else {
                    float ys[2] = {h2 * 0.42f, h2 * 0.58f};
                    if (pos.x >= bX && pos.x <= bX + bW) {
                        if (pos.y >= ys[0] && pos.y <= ys[0] + bH) {
                            humanColor = White;
                            aiColor = Black;
                            return;
                        }
                        if (pos.y >= ys[1] && pos.y <= ys[1] + bH) {
                            humanColor = Black;
                            aiColor = White;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void Renderer::draw(const Board& board) {
    window.clear(sf::Color(40, 40, 40));

    float squareSize = min(window.getSize().x, window.getSize().y) / 8.f;
    float offsetX = (window.getSize().x - squareSize * 8) / 2.f;
    float offsetY = (window.getSize().y - squareSize * 8) / 2.f;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            sf::RectangleShape square({squareSize, squareSize});
            square.setPosition({offsetX + col * squareSize, offsetY + row * squareSize});
            if (row == selectedRow && col == selectedCol)
                square.setFillColor(sf::Color(255, 255, 0));
            else if ((row + col) % 2 == 0)
                square.setFillColor(sf::Color(240, 217, 181));
            else
                square.setFillColor(sf::Color(181, 136, 99));
            window.draw(square);
        }
    }
    if (hasLastPlayedMove){
    sf::RectangleShape square({squareSize, squareSize});
    square.setFillColor(sf::Color(255, 255, 0, 120));
    square.setPosition({offsetX + lastPlayedMove.fromCol * squareSize,offsetY + lastPlayedMove.fromRow * squareSize});
    window.draw(square);
    square.setPosition({offsetX + lastPlayedMove.toCol * squareSize,offsetY + lastPlayedMove.toRow * squareSize});
    window.draw(square);
}
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* piece = board.getPiece(row, col);
            if (piece == nullptr)
                continue;
            char symbol = piece->getSymbol();
            sf::Sprite sprite(textures[symbol]);
            sprite.setPosition({offsetX + col * squareSize, offsetY + row * squareSize});
            auto size = textures[symbol].getSize();
            sprite.setScale({squareSize * 0.9f / size.x, squareSize * 0.9f / size.y});
            sprite.move({squareSize * 0.05f, squareSize * 0.05f});
            window.draw(sprite);
        }
    }

    for (auto move : highlightedMoves) {
        Piece* target = board.getPiece(move.toRow, move.toCol);
        if (target != nullptr) {
            sf::RectangleShape border({squareSize, squareSize});
            border.setPosition({offsetX + move.toCol * squareSize, offsetY + move.toRow * squareSize});
            border.setFillColor(sf::Color::Transparent);
            border.setOutlineColor(sf::Color(0, 255, 0, 200));
            border.setOutlineThickness(-4.f);
            window.draw(border);
        } else {
            sf::CircleShape marker(12.f);
            marker.setFillColor(sf::Color(0, 255, 0, 180));
            marker.setPosition({offsetX + move.toCol * squareSize + squareSize / 2.f - 12.f,
                                offsetY + move.toRow * squareSize + squareSize / 2.f - 12.f});
            window.draw(marker);
        }
    }

    if (gameOver) {
        sf::Text text(font);
        text.setString(gameOverText);
        text.setCharacterSize(static_cast<unsigned int>(squareSize * 0.4f));
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(3.f);
        text.setPosition({offsetX + squareSize, offsetY + squareSize * 3.5f});
        window.draw(text);
    }

    if (board.isPromotionPending()) {
        float x = offsetX + squareSize * 2.5f;
        float y = offsetY + squareSize * 2.f;
        string names[4] = {"Queen", "Rook", "Bishop", "Knight"};
        for (int i = 0; i < 4; i++) {
            sf::RectangleShape box({squareSize * 2.f, squareSize * 0.8f});
            box.setPosition({x, y + i * squareSize});
            box.setFillColor(sf::Color(43, 251, 167));
            window.draw(box);
            sf::Text text(font);
            text.setString(names[i]);
            text.setCharacterSize(static_cast<unsigned int>(squareSize * 0.32f));
            text.setFillColor(sf::Color(0, 0, 0));
            text.setPosition({x + squareSize * 0.25f, y + i * squareSize + squareSize * 0.2f});
            window.draw(text);
        }
    }

    window.display();
}

void Renderer::loadTextures() {
    textures['P'].loadFromFile("assets/whitePawn.png");
    textures['N'].loadFromFile("assets/whiteKnight.png");
    textures['B'].loadFromFile("assets/whiteBishop.png");
    textures['R'].loadFromFile("assets/whiteRook.png");
    textures['Q'].loadFromFile("assets/whiteQueen.png");
    textures['K'].loadFromFile("assets/whiteKing.png");
    textures['p'].loadFromFile("assets/blackPawn.png");
    textures['n'].loadFromFile("assets/blackKnight.png");
    textures['b'].loadFromFile("assets/blackBishop.png");
    textures['r'].loadFromFile("assets/blackRook.png");
    textures['q'].loadFromFile("assets/blackQueen.png");
    textures['k'].loadFromFile("assets/blackKing.png");
}

bool Renderer::handleClick(Board& board, Color& currentTurn, int mouseX, int mouseY) {
    if (gameOver)
        return false;

    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(mouseX, mouseY));
    float squareSize = min(window.getSize().x, window.getSize().y) / 8.f;
    float offsetX = (window.getSize().x - squareSize * 8) / 2.f;
    float offsetY = (window.getSize().y - squareSize * 8) / 2.f;

    if (board.isPromotionPending()) {
        float x = offsetX + squareSize * 2.5f;
        float y = offsetY + squareSize * 2.f;
        bool promoted = false;

        if (worldPos.x >= x && worldPos.x <= x + squareSize * 2.f) {
            if (worldPos.y >= y && worldPos.y <= y + squareSize * 0.8f) {
                board.promoteTo('Q'); promoted = true;
            } else if (worldPos.y >= y + squareSize && worldPos.y <= y + squareSize * 1.8f) {
                board.promoteTo('R'); promoted = true;
            } else if (worldPos.y >= y + squareSize * 2.f && worldPos.y <= y + squareSize * 2.8f) {
                board.promoteTo('B'); promoted = true;
            } else if (worldPos.y >= y + squareSize * 3.f && worldPos.y <= y + squareSize * 3.8f) {
                board.promoteTo('N'); promoted = true;
            }
        }

        if (promoted) {
            currentTurn = currentTurn == White ? Black : White;
            Color nextTurn = currentTurn;
            if (board.isKingInCheck(nextTurn) && board.getAllLegalMoves(nextTurn).empty()) {
                gameOver = true;
                gameOverText = currentTurn == Black ? "Checkmate! White Wins" : "Checkmate! Black Wins";
            } else if (!board.isKingInCheck(nextTurn) && board.getAllLegalMoves(nextTurn).empty()) {
                gameOver = true;
                gameOverText = "Stalemate! Draw";
            }
        }
        return false;
    }

    int row = static_cast<int>((worldPos.y - offsetY) / squareSize);
    int col = static_cast<int>((worldPos.x - offsetX) / squareSize);

    if (row < 0 || row > 7 || col < 0 || col > 7)
        return false;

    if (selectedRow == -1) {
        Piece* piece = board.getPiece(row, col);
        if (piece == nullptr)
            return false;
        if (piece->getColor() != currentTurn)
            return false;
        selectedRow = row;
        selectedCol = col;
        highlightedMoves.clear();
        vector<Move> allMoves = board.getAllLegalMoves(currentTurn);
        for (auto move : allMoves) {
            if (move.fromRow == row && move.fromCol == col)
                highlightedMoves.push_back(move);
        }
        return false;
    }

    for (auto move : highlightedMoves) {
        if (move.toRow == row && move.toCol == col) {
            board.movePiece(move);
            lastPlayedMove=move;hasLastPlayedMove=true;
            if (board.isPromotionPending()) {
                selectedRow = -1;
                selectedCol = -1;
                highlightedMoves.clear();
                return true;
            }

            currentTurn = currentTurn == White ? Black : White;

            if (vsComputer && currentTurn == aiColor && !gameOver) {
                Move aiMove = ai.findBestMove(board, aiColor, 4);
                board.movePiece(aiMove);
                lastPlayedMove=aiMove;hasLastPlayedMove=true;
                if (board.isPromotionPending())
                    board.promoteTo('Q');
                currentTurn = currentTurn == White ? Black : White;
            }

            Color nextTurn = currentTurn;
            if (board.isKingInCheck(nextTurn) && board.getAllLegalMoves(nextTurn).empty()) {
                gameOver = true;
                gameOverText = currentTurn == Black ? "Checkmate! White Wins" : "Checkmate! Black Wins";
            } else if (!board.isKingInCheck(nextTurn) && board.getAllLegalMoves(nextTurn).empty()) {
                gameOver = true;
                gameOverText = "Stalemate! Draw";
            }

            selectedRow = -1;
            selectedCol = -1;
            highlightedMoves.clear();
            return true;
        }
    }

    selectedRow = -1;
    selectedCol = -1;
    highlightedMoves.clear();
    return false;
}