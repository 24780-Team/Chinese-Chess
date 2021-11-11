#include <iostream>
#include <iomanip>
#include "Board.h"

using namespace std;

Board::Board() {
	for (int i = 0; i < width; i++) {
		vector<Piece*> pieces;
		for (int j = 0; j < height; j++) {
			pieces.push_back(nullptr);
		}
		posToPiece.push_back(pieces);
	}

	initializeGeneral();
	initializeAdvisor();
	initializeElephant();
	initializeHorse();
	initializeChariot();
	initializeCannon();
	initializeSoldier();

	general0Alive = true;
	general1Alive = true;
}

Piece* Board::getPiece(Position* pos)
{
	int x = pos->getX();
	int y = pos->getY();
	return posToPiece[x][y];
}

void Board::setPiece(Position* pos, Piece* piece)
{

	Position *prevPos = piece->getPos();
	int x = pos->getX();
	int y = pos->getY();
	int prevX = prevPos->getX();
	int prevY = prevPos->getY();

	piece->setPos(pos);
	Piece* otherPiece = posToPiece[x][y];
	posToPiece[x][y] = piece;
	posToPiece[prevX][prevY] = nullptr;

	if (otherPiece == nullptr) {
		return;
	}
	if (otherPiece->getClassName() == "General") {
		if (piece->getPlayerIndex() == 1) general0Alive = false;
		else general1Alive = false;
	}
	delete otherPiece;
}

vector<Position*> Board::getAvaliblePlaces(Piece* piece)
{
	vector<Position*> positions = piece->getAvaliablePlace(width, height);
	vector<Position*> avaliblePlaces;
	int currPlayerIndex = piece->getPlayerIndex();

	for (auto pos : positions) {
		int x = pos->getX();
		int y = pos->getY();
		Piece* otherPiece = posToPiece[x][y];

		if (otherPiece != nullptr && otherPiece->getPlayerIndex() == currPlayerIndex) {
			continue;
		}

		avaliblePlaces.push_back(pos);
	}
	return avaliblePlaces;
}

int Board::getWinner()
{
	if (!general0Alive) return 0;
	if (!general0Alive) return 1;
	return -1;
}

void Board::draw()
{
	cout << endl;
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			if (!posToPiece[i][j]) cout << setw(10) << "-";
			else cout << setw(10) << left << posToPiece[i][j]->getClassName();
		}
		cout << endl;
	}
}

void Board::initializeGeneral()
{
	Position* pos;
	Piece* piece;

	pos = new Position(4, 0);
	piece = new General();
	piece->setPos(pos);
	piece->setPlayerIndex(0);
	posToPiece[4][0] = piece;

	pos = new Position(4, 9);
	piece = new General();
	piece->setPos(pos);
	piece->setPlayerIndex(1);
	posToPiece[4][9] = piece;
}

void Board::initializeAdvisor()
{
	Position* pos;
	Piece* piece;

	for (int i = -1; i <= 1; i += 2) {
		pos = new Position(4 + i, 0);
		piece = new Advisor();
		piece->setPos(pos);
		piece->setPlayerIndex(0);
		posToPiece[4 + i][0] = piece;

		pos = new Position(4 + i, 9);
		piece = new Advisor();
		piece->setPos(pos);
		piece->setPlayerIndex(1);
		posToPiece[4 + i][9] = piece;
	}
}

void Board::initializeElephant()
{
	Position* pos;
	Piece* piece;

	for (int i = -2; i <= 2; i += 4) {
		pos = new Position(4 + i, 0);
		piece = new Elephant();
		piece->setPos(pos);
		piece->setPlayerIndex(0);
		posToPiece[4 + i][0] = piece;

		pos = new Position(4 + i, 9);
		piece = new Elephant();
		piece->setPos(pos);
		piece->setPlayerIndex(1);
		posToPiece[4 + i][9] = piece;
	}
}

void Board::initializeHorse()
{
	Position* pos;
	Piece* piece;

	for (int i = -3; i <= 3; i += 6) {
		pos = new Position(4 + i, 0);
		piece = new Horse();
		piece->setPos(pos);
		piece->setPlayerIndex(0);
		posToPiece[4 + i][0] = piece;

		pos = new Position(4 + i, 9);
		piece = new Horse();
		piece->setPos(pos);
		piece->setPlayerIndex(1);
		posToPiece[4 + i][9] = piece;
	}
}

void Board::initializeChariot()
{
	Position* pos;
	Piece* piece;

	for (int i = -4; i <= 4; i += 8) {
		pos = new Position(4 + i, 0);
		piece = new Chariot();
		piece->setPos(pos);
		piece->setPlayerIndex(0);
		posToPiece[4 + i][0] = piece;

		pos = new Position(4 + i, 9);
		piece = new Chariot();
		piece->setPos(pos);
		piece->setPlayerIndex(1);
		posToPiece[4 + i][9] = piece;
	}
}

void Board::initializeCannon()
{
	Position* pos;
	Piece* piece;

	for (int i = -3; i <= 3; i += 6) {
		pos = new Position(4 + i, 2);
		piece = new Cannon();
		piece->setPos(pos);
		piece->setPlayerIndex(0);
		posToPiece[4 + i][2] = piece;

		pos = new Position(4 + i, 7);
		piece = new Cannon();
		piece->setPos(pos);
		piece->setPlayerIndex(1);
		posToPiece[4 + i][7] = piece;
	}
}

void Board::initializeSoldier()
{
	Position* pos;
	Piece* piece;

	for (int i = 0; i < 9; i += 2) {
		pos = new Position(i, 3);
		piece = new Soldier();
		piece->setPos(pos);
		piece->setPlayerIndex(0);
		posToPiece[i][3] = piece;

		pos = new Position(i, 6);
		piece = new Soldier();
		piece->setPos(pos);
		piece->setPlayerIndex(1);
		posToPiece[i][6] = piece;
	}
}
