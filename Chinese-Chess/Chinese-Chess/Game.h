#pragma once
#include <string>
#include <stack>
#include <vector>
#include "Player.h"
#include "Board.h"


class Game {
    private:
        Board *board;
        std::vector<Player*> players;
        Player *currPlayer;
        int round;
        int countdown;
        stack<vector<int>> backLog;

        /**
         * @brief record player's actions
         * @param pieceIndex index of Piece moved by player
         * @param originPos original position of the piece
         * @param newPos new position of the piece
         * @param eliminatedPieceIndex index of Piece eliminated by player's Piece, if no elimination, input -1
        */
        void writeLog(int pieceIndex, Position* originPos, Position* newPos, int eliminatedPieceIndex);

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