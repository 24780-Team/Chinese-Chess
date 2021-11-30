#include <iostream>
#include <string>
#include <algorithm>

#include "Game.h"
using namespace std;
using namespace DrawingUtilNG;

void Game::writeLog(int pieceIndex, shared_ptr<Position> originPos, shared_ptr<Position> newPos, int eliminatedPieceIndex)
{
    vector<int> log;
    log.push_back(pieceIndex);
    log.push_back(originPos->getX());
    log.push_back(originPos->getY());
    log.push_back(newPos->getX());
    log.push_back(newPos->getY());
    log.push_back(eliminatedPieceIndex);
    backLog.push(log);
}

void Game::redo()
{
    if (!backLog.empty()) {
        vector<int> log = backLog.top();
        backLog.pop();
        Piece* redoPiece = board->getAlivePieceByIndex(log[0]);
        shared_ptr<Position> originPos(new Position(log[1], log[2]));
        shared_ptr<Position> newPos(new Position(log[3], log[4]));
        setPiece(originPos, redoPiece);
        if (log[5] != -1) {
            Piece* eliminatedPiece = board->getDeadPieceByIndex(log[5]);
            board->setAlive(eliminatedPiece);
        }
    }
}

bool Game::startGame() {
    char bgmPath[] = "Resources/sound/bgm.wav";
    bgm.LoadWav(bgmPath);
    char movePath[] = "Resources/sound/piecemove.wav";
    move.LoadWav(movePath);
    movePlayer.Start();

    bool quit = startStage1();
    if (quit) {
        return true;
    }
    
    board = new Board();
    for (int i = 0; i < 2; i++) {
        Player* player = new Player();
        player->setIndex(i);
        players.push_back(player);
    }

    startStage2();
    currPlayer = players[0];
    round = 1;
    if (hasAI) {
        aiIndex = 1;
        ai = new AIPlayer(board, aiIndex, aiLevel);
        players[0]->setName("Player");
        players[1]->setName("Computer");
    }
    return false;
}

bool Game::endGame(int playerIndex)
{
    cout << endl;
    cout << "Game End." << endl;
    cout << players[playerIndex]->getName() << " win!" << endl;
    return true;
}

bool Game::getWinner(int playerIndex) {
    return board->lossGeneral(1 - playerIndex);
}

bool Game::nextTurn()
{
    int playerIndex = currPlayer->getIndex();
    if (playerIndex == aiIndex) {
        Move* move = ai->getNextMove();
        Piece *piece = board->getPiece(move->origin);
        board->setPiece(move->dest, piece);
        board->setChooseLoc(move->dest);
        delete move;

        if (playerIndex == 1) {
            round += 1;
        }
      
        currPlayer = players[1 - playerIndex];
        if (getWinner(playerIndex)) {
            return endGame(playerIndex);
        }
    }
    else {
        if (nextTurnWithoutAI()) return true;
    }
    return false;
}

bool Game::nextTurnWithoutAI() {

    int mouseEvent, leftButton, middleButton, rightButton;
    int screenX, screenY;

    int playerIndex = currPlayer->getIndex();

    vector<shared_ptr<Position>> placesOfPieces;
    board->getPlacesOfPieces(playerIndex, placesOfPieces);

    Piece* piece = nullptr;
    Piece* eliminatedPiece = nullptr;

    shared_ptr<Position> currentPos = board->getChooseLoc();

    if (!isChoosePiece) {
        for (auto validPos : placesOfPieces) {
            if (currentPos != nullptr && *currentPos == *validPos) {
                isChoosePiece = !isChoosePiece;
                originalPos = currentPos;
                break;
            }
        }
    }
    else {
        piece = board->getPiece(originalPos);
        board->getAvaliblePlaces(piece, avaliablePlaces);
        bool validDest = false;

        for (auto validPos : avaliablePlaces) {
            if (currentPos != nullptr && *currentPos == *validPos) {
                validDest = true;
                break;
            }
        }
        if (validDest) {
            eliminatedPiece = setPiece(currentPos, piece);
            int eliminatedPieceIndex = -1;
            if (eliminatedPiece != nullptr) eliminatedPieceIndex = eliminatedPiece->getPieceIndex();
            writeLog(piece->getPieceIndex(), originalPos, currentPos, eliminatedPieceIndex);
            originalPos = nullptr;
            isChoosePiece = !isChoosePiece;
            board->setIsChoose(false);

            if (playerIndex == 1) {
                round += 1;
            }
            currPlayer = players[1 - playerIndex];
            avaliablePlaces.clear();
        }
        else {
            for (auto validPos : placesOfPieces) {
                if (currentPos != nullptr && *currentPos == *validPos) {
                    originalPos = currentPos;
                    break;
                }
            }
        }
    }

    if (getWinner(playerIndex)) {
        return endGame(playerIndex);
    }

    mouseEvent = FsGetMouseEvent(leftButton, middleButton,
        rightButton, screenX, screenY);

    if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
        board->changeChooseState(screenX, screenY);

        // #6 Background
        // Background color button
        if (board->isInButtons(screenX, screenY) == 1) {
            board->changeBackgroundColor();
        }
        // Background texture button
        else if (board->isInButtons(screenX, screenY) == 2) {
            playMove();
            board->changeBackgroundPic();
        }
        // Piece display pattern
        else if (board->isInButtons(screenX, screenY) == 3) {
            board->changePiecePattern();
        }
        // music control button
        else if (board->isInButtons(screenX, screenY) == 4) {
            if (musicState) {
                pauseBGM();
                musicState = false;
            }
            else {
                resumeBGM();
                musicState = true;
            }
            board->changeMusicButton();
        }
    }

    if (mouseEvent == FSMOUSEEVENT_RBUTTONDOWN) {
        if (isChoosePiece) {
            isChoosePiece = false;
            originalPos = nullptr;
            board->changeChooseState(-1, -1);
        }
        avaliablePlaces.clear();
    }

    return false;
}


Piece* Game::setPiece(shared_ptr<Position> pos, Piece* piece) {
    return board->setPiece(pos, piece);
}

void Game::repentPrevTurn()
{
    return;
}

void Game::showAvaliablePlaces(std::vector<shared_ptr<Position>> avaliablePlaces)
{
    cout << "Avaliable positions are:" << endl;
    for (auto pos : avaliablePlaces) {
        cout << "(" << pos->getX() << "," << pos->getY() << ") ";
    }
    cout << endl;
}

bool Game::startStage1()
{
    int mouseEvent, leftButton, middleButton, rightButton;
    int screenX, screenY;
    int key;
    bool terminate = false;
    YsRawPngDecoder chineseChess, pvp, pve, exit;
    chineseChess.Decode("Resources/words/ChineseChess.png");
    chineseChess.Flip();
    pvp.Decode("Resources/words/pvp.png");
    pvp.Flip();
    pve.Decode("Resources/words/pve.png");
    pve.Flip();
    exit.Decode("Resources/words/exit.png");
    exit.Flip();

    int chineseChessX = width / 2 - 550, chineseChessY = height / 2 - 100, pvpX = width / 2 - 110, pvpY = height / 2 + 50,
        pveX = width / 2 - 110, pveY = height / 2 + 200, exitX = width / 2 - 120, exitY = height / 2 + 350;

    while (!terminate) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glColor3ub(238, 197, 145);
        drawRectangle(0, 0, width, height, true);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(chineseChessX, chineseChessY);
        glDrawPixels(chineseChess.wid, chineseChess.hei, GL_RGBA, GL_UNSIGNED_BYTE, chineseChess.rgba);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(pvpX, pvpY);
        glDrawPixels(pvp.wid, pvp.hei, GL_RGBA, GL_UNSIGNED_BYTE, pvp.rgba);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(pveX, pveY);
        glDrawPixels(pve.wid, pve.hei, GL_RGBA, GL_UNSIGNED_BYTE, pve.rgba);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(exitX, exitY);
        glDrawPixels(exit.wid, exit.hei, GL_RGBA, GL_UNSIGNED_BYTE, exit.rgba);

        FsPollDevice();
        mouseEvent = FsGetMouseEvent(leftButton, middleButton,
            rightButton, screenX, screenY);

        glColor3ub(0, 0, 0);
        if (screenX > pvpX && screenX < pvpX + pvp.wid * 2 / 3 && screenY > pvpY - pvp.hei && screenY < pvpY) {
            drawRectangle(pvpX + 5, pvpY - 163, pvp.wid * 2 / 3, pvp.hei / 2, false);
            if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
                hasAI = false;
                terminate = true;
            }
        }
        else if (screenX > pveX && screenX < pveX + pve.wid * 2 / 3 && screenY > pveY - pve.hei && screenY < pveY) {
            drawRectangle(pveX + 7, pveY - 165, pve.wid * 2 / 3 - 10, pve.hei / 2, false);
            if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
                hasAI = true;
                terminate = true;
            }
        }
        else if (screenX > exitX && screenX < exitX + exit.wid * 2 / 3 && screenY > exitY - exit.hei && screenY < exitY) {
            drawRectangle(exitX + 15, exitY - 165, exit.wid * 2 / 3 - 5, exit.hei / 2, false);
            if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
                return true;
            }
        }

        FsSwapBuffers();
        FsSleep(20);
    }
    return false;
}

void Game::startStage2()
{
    int mouseEvent, leftButton, middleButton, rightButton;
    int screenX, screenY;
    int key;
    bool terminate = false;
    YsRawPngDecoder chineseChess;
    chineseChess.Decode("Resources/words/ChineseChess.png");
    chineseChess.Flip();
    int chineseChessX = width / 2 - 550, chineseChessY = height / 2 - 100;
    if (hasAI) {
        YsRawPngDecoder easy, middum, hard;
        easy.Decode("Resources/words/easy.png");
        easy.Flip();
        middum.Decode("Resources/words/middum.png");
        middum.Flip();
        hard.Decode("Resources/words/hard.png");
        hard.Flip();

        int easyX = width / 2 - 120, easyY = height / 2 + 50, middumX = width / 2 - 210, 
            middumY = height / 2 + 200, hardX = width / 2 - 140, hardY = height / 2 + 350;

        while (!terminate) {
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glColor3ub(238, 197, 145);
            drawRectangle(0, 0, width, height, true);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(chineseChessX, chineseChessY);
            glDrawPixels(chineseChess.wid, chineseChess.hei, GL_RGBA, GL_UNSIGNED_BYTE, chineseChess.rgba);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(easyX, easyY);
            glDrawPixels(easy.wid, easy.hei, GL_RGBA, GL_UNSIGNED_BYTE, easy.rgba);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(middumX, middumY);
            glDrawPixels(middum.wid, middum.hei, GL_RGBA, GL_UNSIGNED_BYTE, middum.rgba);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(hardX, hardY);
            glDrawPixels(hard.wid, hard.hei, GL_RGBA, GL_UNSIGNED_BYTE, hard.rgba);

            FsPollDevice();
            mouseEvent = FsGetMouseEvent(leftButton, middleButton,
                rightButton, screenX, screenY);

            glColor3ub(0, 0, 0);
            if (screenX > easyX && screenX < easyX + easy.wid * 2 / 3 && screenY > easyY - easy.hei && screenY < easyY) {
                drawRectangle(easyX - 5, easyY - 170, easy.wid * 2 / 3, easy.hei / 2 + 5, false);
                if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
                    aiLevel = 2;
                    terminate = true;
                }
            }
            else if (screenX > middumX && screenX < middumX + middum.wid && screenY > middumY - middum.hei && screenY < middumY) {
                drawRectangle(middumX + 15, middumY - 165, middum.wid - 10, middum.hei / 2, false);
                if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
                    aiLevel = 3;
                    terminate = true;
                }
            }
            else if (screenX > hardX && screenX < hardX + hard.wid * 2 / 3 && screenY > hardY - hard.hei && screenY < hardY) {
                drawRectangle(hardX + 10, hardY - 175, hard.wid * 2 / 3 - 5, hard.hei / 2, false);
                if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
                    aiLevel = 4;
                    terminate = true;
                }
            }

            FsSwapBuffers();
            FsSleep(20);
        }
    }
    else {
        getPlayerNameFromScreen(chineseChess);
    }
}

void Game::getPlayerNameFromScreen(YsRawPngDecoder& chineseChess)
{
    int adjustLetter;
    int key;
    string playerName1 = "";
    int chineseChessX = width / 2 - 550, chineseChessY = height / 2 - 100;
    int p1X = 750, p1Y = height / 2;
    int p2X = 750, p2Y = height / 2 + 100;

    FsPollDevice();
    key = FsInkey();
    while (key != FSKEY_ENTER) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glColor3ub(238, 197, 145);
        drawRectangle(0, 0, width, height, true);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(chineseChessX, chineseChessY);
        glDrawPixels(chineseChess.wid, chineseChess.hei, GL_RGBA, GL_UNSIGNED_BYTE, chineseChess.rgba);

        glColor3ub(0, 0, 0);
        glRasterPos2i(p1X - 400, p1Y - 5);
        YsGlDrawFontBitmap20x32("Name of Player1:");
        glRasterPos2i(p2X - 400, p2Y - 5);
        YsGlDrawFontBitmap20x32("Name of Player2:");
        drawRectangle(p1X - 20, p1Y - 45, 300, 50, false);
        drawRectangle(p2X - 20, p2Y - 45, 300, 50, false);

        // build filename from keyboard entry, letter by letter
        buildStringFromFsInkey(key, playerName1);

        playerName1 += "_"; // add an underscore as prompt
        glRasterPos2i(p1X, p1Y);  // sets position
        YsGlDrawFontBitmap20x32(playerName1.c_str());
        playerName1 = playerName1.substr(0, playerName1.length() - 1); // remove underscore

        FsSwapBuffers();
        FsSleep(25);

        FsPollDevice();
        key = FsInkey();
    }

    if (playerName1 == "") {
        players[0]->setName("Player1");
    }
    else {
        players[0]->setName(playerName1);
    }
    
    string playerName2 = "";
    FsPollDevice();
    key = FsInkey();
    while (key != FSKEY_ENTER) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glColor3ub(238, 197, 145);
        drawRectangle(0, 0, width, height, true);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(chineseChessX, chineseChessY);
        glDrawPixels(chineseChess.wid, chineseChess.hei, GL_RGBA, GL_UNSIGNED_BYTE, chineseChess.rgba);

        // ask for file name from the graphics window
        glColor3ub(0, 0, 0);
        glRasterPos2i(p1X - 400, p1Y - 5);
        YsGlDrawFontBitmap20x32("Name of Player1:");
        glRasterPos2i(p1X, p1Y);
        YsGlDrawFontBitmap20x32(playerName1.c_str());
        glRasterPos2i(p2X - 400, p2Y - 5);
        YsGlDrawFontBitmap20x32("Name of Player2:");
        drawRectangle(p2X - 20, p2Y - 45, 300, 50, false);

        // build filename from keyboard entry, letter by letter
        buildStringFromFsInkey(key, playerName2);

        playerName2 += "_"; // add an underscore as prompt
        glRasterPos2i(p2X, p2Y);  // sets position
        YsGlDrawFontBitmap20x32(playerName2.c_str());
        playerName2 = playerName2.substr(0, playerName2.length() - 1); // remove underscore

        FsSwapBuffers();
        FsSleep(25);

        FsPollDevice();
        key = FsInkey();
    }

    if (playerName2 == "") {
        players[1]->setName("Player2");
    }
    else {
        players[1]->setName(playerName2);
    }
}

void Game::draw()
{
    board->drawBoard();
    board->drawButtons();
    board->drawPieces();
    board->drawPlayerInformation();
    board->drawCurrentFrame();
    board->drawPlayerFrame(currPlayer->getIndex(), players[0]->getName(), players[1]->getName());
    board->drawNodes(avaliablePlaces);
}


