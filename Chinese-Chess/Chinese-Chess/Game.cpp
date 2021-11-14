#include <iostream>

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

void Game::startGame() {
    board = new Board();
    for (int i = 0; i < 2; i++) {
        Player* player = new Player();
        player->setIndex(i);
        player->setName();
        players.push_back(player);
    }
    currPlayer = players[0];
    round = 1;
}

int Game::getWinner() {
    return board->getWinner();
}

bool Game::nextTurn() {
    cout << endl;
    cout << "Round " << round << endl;

    int playerIndex = currPlayer->getIndex();
    cout << "Current Player: " << currPlayer->getName() << endl;
    vector<Position*> placesOfPieces = board->getPlacesOfPieces(playerIndex);
    showAvaliablePlaces(placesOfPieces);

    bool isSamePlace = true;
    Piece* piece = nullptr;
    Piece* eliminatedPiece = nullptr;
    Position* originalPos = nullptr;
    Position* newPos = nullptr;

    while (isSamePlace) {
        originalPos = getPosition(placesOfPieces);
        piece = board->getPiece(originalPos);

        vector<Position*> avaliablePlaces = board->getAvaliblePlaces(piece);
        showAvaliablePlaces(avaliablePlaces);

        newPos = getPosition(avaliablePlaces);
        if (newPos != originalPos) {
            eliminatedPiece = setPiece(newPos, piece);
            isSamePlace = false;
        }
        else {
            cout << "The piece was put down, please select again.\n" << endl;
        }
    }
    
    int eliminatedPieceIndex = -1;
    if (eliminatedPiece != nullptr) eliminatedPieceIndex = eliminatedPiece->getPieceIndex();
    writeLog(piece->getPieceIndex(), originalPos, newPos, eliminatedPieceIndex);

    if (getWinner() != -1) {
        return true;
    }

    if (playerIndex == 1) {
        round += 1;
    }
    currPlayer = players[1 - playerIndex];
    return false;
}

Position* Game::getPosition(vector<Position*> positions)
{
    int x, y;
    bool isValidInput = false;
    Position* pos = nullptr;

    while (!isValidInput) {
        cout << "Input a position:" << endl;
        cout << "x = ";
        cin >> x;
        cout << "y = ";
        cin >> y;
        for (auto it = positions.begin(); it != positions.end(); it++)
        {
            if ((*it)->getX() == x && (*it)->getY() == y) {
                pos = *it;
                isValidInput = true;
                break;
            }
        }
        if (!isValidInput) {
            cout << "Not a valid position.\n" << endl;
        }
    }
    return pos;
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
    board->draw();
}
