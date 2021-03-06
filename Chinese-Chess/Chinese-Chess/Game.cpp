#include <iostream>
#include <algorithm>
#include "Game.h"
using namespace std;

void Game::writeLog(int pieceIndex, Position* originPos, Position* newPos, int eliminatedPieceIndex)
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
        Position* originPos = new Position(log[1], log[2]);
        Position* newPos = new Position(log[3], log[4]);
        setPiece(originPos, redoPiece);
        if (log[5] != -1) {
            Piece* eliminatedPiece = board->getDeadPieceByIndex(log[5]);
            setPiece(newPos, eliminatedPiece);
        }
    }
}

void Game::startGame() {
    board = new Board();
    for (int i = 0; i < 2; i++) {
        Player* player = new Player();
        player->setIndex(i);
        //player->setName();
        players.push_back(player);
    }
    currPlayer = players[0];
    round = 1;
}

int Game::getWinner() {
    return board->getWinner();
}

bool Game::nextTurn() {

    int mouseEvent, leftButton, middleButton, rightButton;
    int screenX, screenY;

    int playerIndex = currPlayer->getIndex();

    vector<Position*> placesOfPieces = board->getPlacesOfPieces(playerIndex);

    Piece* piece = nullptr;
    Piece* eliminatedPiece = nullptr;

    Position* currentPos = board->getChooseLoc();

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
        vector<Position*> avaliablePlaces = board->getAvaliblePlaces(piece);
        bool validDest = false;

        board->drawNodes(avaliablePlaces);
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

    int winner;
    if ((winner = getWinner()) != -1) {
        cout << endl;
        cout << "Game End." << endl;
        cout << getPlayerName(players[winner]) << " win!" << endl;
        return true;
    }

    mouseEvent = FsGetMouseEvent(leftButton, middleButton,
        rightButton, screenX, screenY);

    if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
        board->changeChooseState(screenX, screenY);
        if (board->isChooseLocationInChangePattern(screenX, screenY)) {
            mode = 1 - mode;
        }
    }

    if (mouseEvent == FSMOUSEEVENT_RBUTTONDOWN) {
        if (isChoosePiece) {
            isChoosePiece = false;
            originalPos = nullptr;
            board->changeChooseState(-1, -1);
        }
    }

    return false;
}


Piece* Game::setPiece(Position* pos, Piece* piece) {
    return board->setPiece(pos, piece);
}

void Game::repentPrevTurn()
{
    return;
}

void Game::showAvaliablePlaces(std::vector<Position*> avaliablePlaces)
{
    cout << "Avaliable positions are:" << endl;
    for (auto pos : avaliablePlaces) {
        cout << "(" << pos->getX() << "," << pos->getY() << ") ";
    }
    cout << endl;
}

void Game::draw()
{
    board->drawBoard();
    board->drawPieces(mode);
    board->drawCurrentFrame();
    board->drawPlayerFrame(currPlayer->getIndex());
    board->drawModeChooseFrame();
}


