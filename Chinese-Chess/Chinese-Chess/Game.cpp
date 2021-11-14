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

    int playIndex = currPlayer->getIndex();
    cout << "Current Player: " << currPlayer->getName() << endl;
    Position *pos = getPosition();
    Piece *piece = board->getPiece(pos);

    vector<Position*> avaliablePlaces = board->getAvaliblePlaces(piece);
    showAvaliablePlaces(avaliablePlaces);

    pos = getPosition();
    setPiece(pos, piece);

    writeLog(piece->getPieceIndex(), originPos, newPos, eliminatedPieceIndex);

    if (getWinner() != -1) {
        return true;
    }

    
    if (playIndex == 1) {
        round += 1;
    }
    currPlayer = players[1 - playIndex];
    return false;
}

Position* Game::getPosition()
{
    int x, y;
    cout << "Input a position:" << endl;
    cout << "x = ";
    cin >> x;
    cout << "y = ";
    cin >> y;
    Position* pos = new Position(x, y);
    return pos;
}

void Game::setPiece(Position* pos, Piece* piece) {
    board->setPiece(pos, piece);
}

void Game::repentPrevTurn()
{
    return;
}

void Game::showAvaliablePlaces(std::vector<Position*> avaliablePlaces)
{
    for (auto pos : avaliablePlaces) {
        cout << "(" << pos->getX() << "," << pos->getY() << ") ";
    }
    cout << endl;
}

void Game::draw()
{
    board->draw();
}
