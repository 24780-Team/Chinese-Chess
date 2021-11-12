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

	int index = 0;
	initializeGeneral(index);
	initializeAdvisor(index);
	initializeElephant(index);
	initializeHorse(index);
	initializeChariot(index);
	initializeCannon(index);
	initializeSoldier(index);

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
	if (otherPiece->getType() == pieceType::GENERAL) {
		if (piece->getPlayerIndex() == 1) general0Alive = false;
		else general1Alive = false;
	}
	delete otherPiece;
}

void Board::addPiece(Piece* piece)
{
	return;
}

vector<Position*> Board::getAvaliblePlaces(Piece* piece)
{
	vector<Position*> positions = piece->getAvaliablePlace(width, height);
	vector<Position*> avaliblePlaces;
	int currPlayerIndex = piece->getPlayerIndex();

	// Handle four special cases
	switch (piece->getType()) {
	case pieceType::ELEPHANT:
		positions = checkForElephant(piece, positions);
		break;
	case pieceType::HORSE:
		positions = checkForHorse(piece, positions);
		break;
	case pieceType::CHARIOT:
		positions = checkForChariot(piece);
		break;
	case pieceType::CANNON:
		positions = checkForCannon(piece);
		break;
	}

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
			else {
				switch (posToPiece[i][j]->getType()) {
				case pieceType::GENERAL:
					cout << setw(10) << left << "GENERAL";
					break;
				case pieceType::ADVISOR:
					cout << setw(10) << left << "ADVISOR";
					break;
				case pieceType::ELEPHANT:
					cout << setw(10) << left << "ELEPHANT";
					break;
				case pieceType::HORSE:
					cout << setw(10) << left << "HORSE";
					break;
				case pieceType::CHARIOT:
					cout << setw(10) << left << "CHARIOT";
					break;
				case pieceType::CANNON:
					cout << setw(10) << left << "CANNON";
					break;
				case pieceType::SOLDIER:
					cout << setw(10) << left << "SOLDIER";
					break;
				}
			}
		}
		cout << endl;
	}
}

void Board::initializeGeneral(int& index)
{
	Position* pos;
	Piece* piece;

	pos = new Position(4, 0);
	piece = new General(4, 0, 0, index);
	index += 1;
	posToPiece[4][0] = piece;
	
	pos = new Position(4, 9);
	piece = new General(4, 9, 1, index);
	index += 1;
	posToPiece[4][9] = piece;
}

void Board::initializeAdvisor(int& index)
{
	Position* pos;
	Piece* piece;

	for (int i = -1; i <= 1; i += 2) {
		pos = new Position(4 + i, 0);
		piece = new Advisor(4 + i, 0, 0, index);
		index += 1;
		posToPiece[4 + i][0] = piece;

		pos = new Position(4 + i, 9);
		piece = new Advisor(4 + i, 9, 1, index);
		index += 1;
		posToPiece[4 + i][9] = piece;
	}
}

void Board::initializeElephant(int& index)
{
	Position* pos;
	Piece* piece;

	for (int i = -2; i <= 2; i += 4) {
		pos = new Position(4 + i, 0);
		piece = new Elephant(4 + i, 0, 0, index);
		index += 1;
		posToPiece[4 + i][0] = piece;

		pos = new Position(4 + i, 9);
		piece = new Elephant(4 + i, 9, 1, index);
		index += 1;
		posToPiece[4 + i][9] = piece;
	}
}

void Board::initializeHorse(int& index)
{
	Position* pos;
	Piece* piece;

	for (int i = -3; i <= 3; i += 6) {
		pos = new Position(4 + i, 0);
		piece = new Horse(4 + i, 0, 0, index);
		index += 1;
		posToPiece[4 + i][0] = piece;

		pos = new Position(4 + i, 9);
		piece = new Horse(4 + i, 9, 1, index);
		index += 1;
		posToPiece[4 + i][9] = piece;
	}
}

void Board::initializeChariot(int& index)
{
	Position* pos;
	Piece* piece;

	for (int i = -4; i <= 4; i += 8) {
		pos = new Position(4 + i, 0);
		piece = new Chariot(4 + i, 0, 0, index);
		index += 1;
		posToPiece[4 + i][0] = piece;

		pos = new Position(4 + i, 9);
		piece = new Chariot(4 + i, 9, 1, index);
		index += 1;
		posToPiece[4 + i][9] = piece;
	}
}

void Board::initializeCannon(int& index)
{
	Position* pos;
	Piece* piece;

	for (int i = -3; i <= 3; i += 6) {
		pos = new Position(4 + i, 2);
		piece = new Cannon(4 + i, 2, 0, index);
		index += 1;
		posToPiece[4 + i][2] = piece;

		pos = new Position(4 + i, 7);
		piece = new Cannon(4 + i, 7, 1, index);
		index += 1;
		posToPiece[4 + i][7] = piece;
	}
}

void Board::initializeSoldier(int& index)
{
	Position* pos;
	Piece* piece;

	for (int i = 0; i < 9; i += 2) {
		pos = new Position(i, 3);
		piece = new Soldier(i, 3, 0, index);
		index += 1;
		posToPiece[i][3] = piece;

		pos = new Position(i, 6);
		piece = new Soldier(i, 6, 1, index);
		index += 1;
		posToPiece[i][6] = piece;
	}
}

vector<Position*> Board::checkForElephant(Piece* piece, vector<Position*>& positions)
{
	int currX = piece->getPos()->getX();
	int currY = piece->getPos()->getY();
	
	for (auto iter = positions.begin(); iter != positions.end();) {
		Position* pos = *iter;
		int dx = pos->getX() - currX;
		int dy = pos->getY() - currY;
		if (posToPiece[currX + dx / 2][currY + dy / 2] != nullptr) {
			iter = positions.erase(iter);
		}
		else {
			iter++;
		}
	}
	return positions;
}

vector<Position*> Board::checkForHorse(Piece* piece, vector<Position*>& positions)
{
	int currX = piece->getPos()->getX();
	int currY = piece->getPos()->getY();

	for (auto iter = positions.begin(); iter != positions.end();) {
		Position* pos = *iter;
		int dx = pos->getX() - currX;
		int dy = pos->getY() - currY;
		if (posToPiece[currX + dx / 2][currY + dy / 2] != nullptr) {
			iter = positions.erase(iter);
		}
		else {
			iter++;
		}
	}
	return positions;
}

std::vector<Position*> Board::checkForGeneral(Piece* piece, std::vector<Position*>& positions)
{
	int x = piece->getPos()->getX();
	int y = piece->getPos()->getY();
	int playerIndex = piece->getPlayerIndex();
	if (playerIndex == 0) {
		for (int j = y + 1; j < height; j++) {
			if (posToPiece[x][j] != nullptr) {
				Piece* piece = posToPiece[x][j];
				if (piece->getType() == pieceType::GENERAL) {
					Position* pos = new Position(x, j);
					positions.push_back(pos);
				}
				break;
			}
		}
	}
	else {
		for (int j = y - 1; j >= 0; j--) {
			if (posToPiece[x][j] != nullptr) {
				Piece* piece = posToPiece[x][j];
				if (piece->getType() == pieceType::GENERAL) {
					Position* pos = new Position(x, j);
					positions.push_back(pos);
				}
				break;
			}
		}
	}
}

vector<Position*> Board::checkForChariot(Piece* piece)
{
	vector<Position*> avaliablePlace;
	int x = piece->getPos()->getX();
	int y = piece->getPos()->getY();
	int playerIndex = piece->getPlayerIndex();
	for (int i = x + 1; i < width; i++) {
		Position* pos;
		pos = new Position(i, y);
		if (posToPiece[i][y] == nullptr) {
			avaliablePlace.push_back(pos);
		}
		else {
			Piece* piece = posToPiece[i][y];
			if (piece->getPlayerIndex() != playerIndex) {
				avaliablePlace.push_back(pos);
			}
			break;
		}
	}

	for (int i = x - 1; i >= 0; i--) {
		Position* pos;
		pos = new Position(i, y);
		if (posToPiece[i][y] == nullptr) {
			avaliablePlace.push_back(pos);
		}
		else {
			Piece* piece = posToPiece[i][y];
			if (piece->getPlayerIndex() != playerIndex) {
				avaliablePlace.push_back(pos);
			}
			break;
		}
	}

	for (int j = y + 1; j < height; j++) {
		Position* pos;
		pos = new Position(x, j);
		if (posToPiece[x][j] == nullptr) {
			avaliablePlace.push_back(pos);
		}
		else {
			Piece* piece = posToPiece[x][j];
			if (piece->getPlayerIndex() != playerIndex) {
				avaliablePlace.push_back(pos);
			}
			break;
		}
	}

	for (int j = y - 1; j >= 0; j--) {
		Position* pos;
		pos = new Position(x, j);
		if (posToPiece[x][j] == nullptr) {
			avaliablePlace.push_back(pos);
		}
		else {
			Piece* piece = posToPiece[x][j];
			if (piece->getPlayerIndex() != playerIndex) {
				avaliablePlace.push_back(pos);
			}
			break;
		}
	}

	return avaliablePlace;
}

vector<Position*> Board::checkForCannon(Piece* piece)
{
	vector<Position*> avaliablePlace;
	int x = piece->getPos()->getX();
	int y = piece->getPos()->getY();
	int playerIndex = piece->getPlayerIndex();
	bool meetObstacle = false;

	for (int i = x + 1; i < width; i++) {
		Position* pos;
		pos = new Position(i, y);
		if (posToPiece[i][y] == nullptr) {
			if (meetObstacle == false) {
				avaliablePlace.push_back(pos);
			}
		}
		else {
			if (meetObstacle == false) {
				meetObstacle = true;
				continue;
			}
			else {
				Piece* piece = posToPiece[i][y];
				if (piece->getPlayerIndex() != playerIndex) {
					avaliablePlace.push_back(pos);
				}
				break;
			}
		}
	}

	meetObstacle = false;

	for (int i = x - 1; i >= 0; i--) {
		Position* pos;
		pos = new Position(i, y);
		if (posToPiece[i][y] == nullptr) {
			if (!meetObstacle) {
				avaliablePlace.push_back(pos);
			}
		}
		else {
			if (!meetObstacle) {
				meetObstacle = true;
				continue;
			}
			else {
				Piece* piece = posToPiece[i][y];
				if (piece->getPlayerIndex() != playerIndex) {
					avaliablePlace.push_back(pos);
				}
				break;
			}
		}
	}

	meetObstacle = false;

	for (int j = y + 1; j < height; j++) {
		Position* pos;
		pos = new Position(x, j);
		if (posToPiece[x][j] == nullptr) {
			if (!meetObstacle) {
				avaliablePlace.push_back(pos);
			}
		}
		else {
			if (!meetObstacle) {
				meetObstacle = true;
				continue;
			}
			else {
				Piece* piece = posToPiece[x][j];
				if (piece->getPlayerIndex() != playerIndex) {
					avaliablePlace.push_back(pos);
				}
				break;
			}
		}
	}

	meetObstacle = false;

	for (int j = y - 1; j >= 0; j--) {
		Position* pos;
		pos = new Position(x, j);
		if (posToPiece[x][j] == nullptr) {
			if (!meetObstacle) {
				avaliablePlace.push_back(pos);
			}
		}
		else {
			if (!meetObstacle) {
				meetObstacle = true;
				continue;
			}
			else {
				Piece* piece = posToPiece[x][j];
				if (piece->getPlayerIndex() != playerIndex) {
					avaliablePlace.push_back(pos);
				}
				break;
			}
		}
	}

	return avaliablePlace;
}
