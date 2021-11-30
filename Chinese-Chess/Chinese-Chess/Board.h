#pragma once
#include <vector>
#include <unordered_map>

#include "ysglfontdata.h"
#include "fssimplewindow.h"
#include "StringPlus.h"
#include "yspng.h"
#include "DrawingUtilNG.h"
#include "yssimplesound.h"

#include "Piece.h"

class Board {
private:
    std::vector<std::vector<Piece*>> posToPiece;
    int width = 9;
    int height = 10;

    int general0Index;
    int general1Index;

    // #6 Background
    int backgroundColor = 1;
    int backgroundPic = 1;
    // 0 for Picture, 1 for Chinese
    int piecesPattern = 1;
    bool music_on = true;

    YsRawPngDecoder backgroundTexture1;
    YsRawPngDecoder backgroundTexture2;
    YsRawPngDecoder playerInformBoard;
    YsRawPngDecoder backgroundColorButton;
    YsRawPngDecoder backgroundPicButton;
    YsRawPngDecoder piecePatternButton;
    YsRawPngDecoder musicOnButton;
    YsRawPngDecoder musicOffButton;

    std::unordered_map<int, Piece*> player0Alive;
    std::unordered_map<int, Piece*> player1Alive;
    std::unordered_map<int, Piece*> player0Dead;
    std::unordered_map<int, Piece*> player1Dead;

    void initializeGeneral(int& index);
    void initializeAdvisor(int& index);
    void initializeElephant(int& index);
    void initializeHorse(int& index);
    void initializeChariot(int& index);
    void initializeCannon(int& index);
    void initializeSoldier(int& index);

    std::vector<shared_ptr<Position>> checkForElephant(Piece* piece, std::vector<shared_ptr<Position>>& positions);
    std::vector<shared_ptr<Position>> checkForHorse(Piece* piece, std::vector<shared_ptr<Position>>& positions);
    std::vector<shared_ptr<Position>> checkForGeneral(Piece* piece, std::vector<shared_ptr<Position>>& positions);
    std::vector<shared_ptr<Position>> checkForChariot(Piece* piece);
    std::vector<shared_ptr<Position>> checkForCannon(Piece* piece);


    // used to show chosen location
    bool isChoose;
    shared_ptr<Position> chooseLoc;

    // used to show where the pieces can be moved to 
    int rgbr = 135;
    int rgbg = 205;
    int rgbb = 235;
    int update = -1;

public:
    Board();
    Piece* getPiece(shared_ptr<Position> pos);
    Piece* setPiece(shared_ptr<Position> pos, Piece* piece);
    void getPlacesOfPieces(int playerIndex, vector<shared_ptr<Position>>& places);
    void getAvaliblePlaces(Piece* piece, vector<shared_ptr<Position>>& places);
    Piece* getAlivePieceByIndex(int index);
    Piece* getDeadPieceByIndex(int index);
    bool lossGeneral(int playerIndex);

    void draw(); // Need to be changed.

    // #5 Timer
    void drawPlayerInformation();

    void drawBoard();

    // #6 Background
    void drawPieces();

    void drawCurrentFrame();
    // draw the choose frame
    void drawChooseFrame(shared_ptr<Position> theLoc);

    void drawModeChooseFrame();

    void drawPlayerFrame(int currPlayerIndex, string nameOfP0, string nameOfP1);

    void drawNodes(const vector<shared_ptr<Position>>& avaliablePlaces);

    bool isChooseLocationInBoard(int screenX, int screenY);

    void changeChooseState(int screenX, int screenY);

    void setIsChoose(bool isChoose) {
        this->isChoose = isChoose;
    }

    shared_ptr<Position> getChooseLoc() {
        return chooseLoc;
    }

    void setChooseLoc(shared_ptr<Position> pos) {
        this->isChoose = true;
        chooseLoc = pos;
    }

    shared_ptr<Position> getChooseLocation(int screenX, int screenY);

    // Calculate score for AI
    int calcScore(int playerIndex);

    void setAlive(Piece* piece);

    // #6 Background
    void setBackgroundColor();

    void setBackgroundPic();

    void changeBackgroundColor();

    void changeBackgroundPic();

    void changePiecePattern() { piecesPattern = 1 - piecesPattern; }

    void changeMusicButton() { music_on = !music_on; }

    void drawButtons();

    int isInButtons(int screenX, int screenY);
};