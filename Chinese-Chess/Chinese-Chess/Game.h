#pragma once
#include <string>

#include "Player.h"
#include "Board.h"

class Game {
    private:
        Board *board;
        std::vector<Player*> players;
        Player *currPlayer;
        int round;
        int countdown;

    public:
        std::string getPlayerName(Player *player) { return player->getName(); }
        void startGame();
        int getWinner();
        bool nextTurn();
        Position* getPosition(std::vector<Position*> positions); // Need to be changed.
        void setPiece(Position* pos, Piece* piece);
        void repentPrevTurn(); // Need to be implemented.

        void showAvaliablePlaces(std::vector<Position*> avaliablePlaces); // Need to be changed.
        void draw(); // Need to be changed.
};