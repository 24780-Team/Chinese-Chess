#pragma once
#include <vector>
#include <unordered_map>

#include "ysglfontdata.h"
#include "fssimplewindow.h"
#include "StringPlus.h"
#include "yspng.h"
#include "DrawingUtilNG.h"

#include "Piece.h"

class Board {
    private:
        std::vector<std::vector<Piece*>> posToPiece;
        int width = 9;
        int height = 10;

        int general0Index;
        int general1Index;

        std::unordered_map<int, Piece*> player0Alive;
        std::unordered_map<int, Piece*> player1Alive;
        std::unordered_map<int, Piece*> player0Dead;
        std::unordered_map<int, Piece*> player1Dead;

        void initializeGeneral(int &index);
        void initializeAdvisor(int& index);
        void initializeElephant(int& index);
        void initializeHorse(int& index);
        void initializeChariot(int& index);
        void initializeCannon(int& index);
        void initializeSoldier(int& index);

        std::vector<Position*> checkForElephant(Piece* piece, std::vector<Position*>& positions);
        std::vector<Position*> checkForHorse(Piece* piece, std::vector<Position*>& positions);
        std::vector<Position*> checkForGeneral(Piece* piece, std::vector<Position*>& positions);
        std::vector<Position*> checkForChariot(Piece* piece);
        std::vector<Position*> checkForCannon(Piece* piece);

        //void addPiece(const std::string filename, int playerIndex, int startIndex);

        // used to show chosen location
        bool isChoose;
        Position *chooseLoc;

        // used to show where the pieces can be moved to 
        int rgbr = 135;
        int rgbg = 205;
        int rgbb = 235;
        int update = -1;
    
    public:
        Board();
        Piece* getPiece(Position* pos);
        Piece* setPiece(Position* pos, Piece* piece);
        std::vector<Position*> getPlacesOfPieces(int playerIndex);
        std::vector<Position*> getAvaliblePlaces(Piece *piece);
        Piece* getAlivePieceByIndex(int index);
        Piece* getDeadPieceByIndex(int index);
        int getWinner();

        void draw(); // Need to be changed.
        void drawBoard();
        void drawPieces(int mode);

        void drawCurrentFrame();
        // draw the choose frame
        void drawChooseFrame(Position* theLoc);

        void drawModeChooseFrame();



        bool isChooseLocationInBoard(int screenX, int screenY);

        void changeChooseState(int screenX, int screenY);

        void setIsChoose(bool isChoose) {
            this->isChoose = isChoose;
        }

        Position* getChooseLoc() {
            return chooseLoc;
        }

        Position* getChooseLocation(int screenX, int screenY);

        bool isChooseLocationInChangePattern(int screenX, int screenY);
};