#pragma once
#include <string>
#include <stack>
#include <vector>

#include "fssimplewindow.h"
#include "ysglfontdata.h"

#include "Player.h"
#include "Board.h"
#include "AIPlayer.h"

class Game {
    private:
        Board *board;
        AIPlayer *ai;

        std::vector<Player*> players;
        Player *currPlayer;
        int round;
        int countdown;
        stack<vector<int>> backLog;
        int mode = 1;
        bool isChoosePiece = false;

        Position* originalPos = nullptr;

        bool haveAI = true;
        bool aiLevel = 1; // The higher, the smarter. Level 0 means randomly choose a piece and a destination.
        int aiIndex = -1; // AI's player index

        /**
         * @brief record player's actions
         * @param pieceIndex index of Piece moved by player
         * @param originPos original position of the piece
         * @param newPos new position of the piece
         * @param eliminatedPieceIndex index of Piece eliminated by player's Piece, if no elimination, input -1
        */
        void writeLog(int pieceIndex, Position* originPos, Position* newPos, int eliminatedPieceIndex);

        bool nextTurnWithoutAI();
        void redo();

    public:
        std::string getPlayerName(Player *player) { return player->getName(); }
        void startGame();
        bool getWinner(int playerIndex);
        bool nextTurn();
        Piece* setPiece(Position* pos, Piece* piece);
        void repentPrevTurn(); // Need to be implemented.

        void showAvaliablePlaces(std::vector<Position*> avaliablePlaces); // Need to be changed.
        void draw(); 
};