#pragma once
#include <string>
#include <stack>
#include <vector>

#include "fssimplewindow.h"
#include "ysglfontdata.h"

#include "Player.h"
#include "Board.h"
#include "AIPlayer.h"

const int width = 1400, height = 1200;

class Game {
    private:
        Board* board;
        AIPlayer* ai;

        bool isTerminate = false;
        std::vector<Player*> players;
        Player*currPlayer;
        int round;
        int countdown;
        stack<vector<int>> backLog;
        int mode = 1;
        bool isChoosePiece = false;

        shared_ptr<Position> originalPos = nullptr;

        vector<shared_ptr<Position>> avaliablePlaces;

        bool hasAI = true;
        int aiLevel = 0; // The higher, the smarter. Level 0 means randomly choose a piece and a destination.
        int aiIndex = -1; // AI's player index

        bool musicState = true;
        YsSoundPlayer bgmPlayer;
        YsSoundPlayer::SoundData bgm;
        YsSoundPlayer movePlayer;
        YsSoundPlayer::SoundData move;
        ComicSansFont comicsans;

        /**
         * @brief record player's actions
         * @param pieceIndex index of Piece moved by player
         * @param originPos original position of the piece
         * @param newPos new position of the piece
         * @param eliminatedPieceIndex index of Piece eliminated by player's Piece, if no elimination, input -1
        */
        void writeLog(int pieceIndex, shared_ptr<Position> originPos, shared_ptr<Position> newPos, int eliminatedPieceIndex);

        bool nextTurnWithoutAI();
        bool redo();
        bool startStage1();
        void startStage2();
        void getPlayerNameFromScreen(YsRawPngDecoder& chineseChess);
        void endGame(int playerIndex);

    public:
        std::string getPlayerName(Player *player) { return player->getName(); }
        bool startGame();
        bool getWinner(int playerIndex);
        bool nextTurn();
        Piece* setPiece(shared_ptr<Position> pos, Piece* piece);
        void repentPrevTurn(); // Need to be implemented.
        void restart();
        bool isTerminated() { return this->isTerminate; }

        void draw(); 

        // #7 BGM
        void playBGM() {
            bgmPlayer.Start();
            bgmPlayer.PlayBackground(bgm);
        }

        void pauseBGM() { bgmPlayer.Pause(bgm); }

        void resumeBGM() { bgmPlayer.Resume(bgm); }

        void playMove() {

            movePlayer.PlayOneShot(move);
            // movePlayer.End();
        }
};