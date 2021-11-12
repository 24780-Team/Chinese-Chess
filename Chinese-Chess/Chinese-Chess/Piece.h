#pragma once

#include "Position.h"
#include <vector>

using namespace std;

enum pieceType {BASE, GENERAL, ADVISOR, ELEPHANT, HORSE, CHARIOT, CANNON, SOLDIER};

class Piece {
protected:
	Position* pos;
	int playerIndex;
	int pieceIndex;
	bool isCrossRiver;
	vector<int[]> steps;
	pieceType type;
public:
	Piece(int x, int y, int playerIndex, int pieceIndex) {
		this->pos = new Position(x, y);
		this->playerIndex = playerIndex;
		this->pieceIndex = pieceIndex;
		this->isCrossRiver = false;
		this->type = BASE;
	}
	Position* getPos() { return pos; };
	void setPos(Position* pos) { this->pos = pos; };
	int getPlayerIndex() { return playerIndex; };
	pieceType getType() { return type; };
	virtual vector<Position*> getAvaliablePlace(int width, int height);
	virtual bool onBoard(Position* pos);

	void draw();
};


class General : public Piece {
public:
	General(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		steps.push_back({ 1, 0 });
		steps.push_back({ -1, 0 });
		steps.push_back({ 0, 1 });
		steps.push_back({ 0, -1 });
		this->type = GENERAL;
	};
	virtual vector<Position*> getAvaliablePlace(int width, int height);
	virtual bool onBoard(Position* pos);
};

class Advisor : public Piece {
public:
	Advisor(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		steps.push_back({ 1, 1 });
		steps.push_back({ -1, 1 });
		steps.push_back({ 1, -1 });
		steps.push_back({ -1, -1 });
		this->type = ADVISOR;
	};
	virtual bool onBoard(Position* pos);
	virtual vector<Position*> getAvaliablePlace(int width, int height);
};

class Elephant : public Piece {
public:
	Elephant(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		steps.push_back({ 2, 2 });
		steps.push_back({ -2, 2 });
		steps.push_back({ 2, -2 });
		steps.push_back({ -2, -2 });
		this->type = ELEPHANT;
	};
	virtual bool onBoard(Position* pos);
	virtual vector<Position*> getAvaliablePlace(int width, int height);
};

class Horse : public Piece {
public:
	Horse(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		steps.push_back({ 1, 2 });
		steps.push_back({ 2, 1 });
		steps.push_back({ -1, 2 });
		steps.push_back({ -2, 1 });
		steps.push_back({ -1, -2 });
		steps.push_back({ -2, -1 });
		steps.push_back({ 1, -2 });
		steps.push_back({ 2, -1 });
		this->type = HORSE;
	};
	virtual vector<Position*> getAvaliablePlace(int width, int height);
};

class Chariot : public Piece {
public:
	Chariot(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		this->type = CHARIOT;
	};
	virtual vector<Position*> getAvaliablePlace(int width, int height);
};

class Cannon : public Piece {
public:
	Cannon(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		this->type = CANNON;
	};
	virtual vector<Position*> getAvaliablePlace(int width, int height);
};

class Soldier : public Piece {
public:
	Soldier(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		steps.push_back({ 0, 1 });
		steps.push_back({ 1, 0 });
		steps.push_back({ -1, 0 });
		this->type = SOLDIER;
	};
	virtual vector<Position*> getAvaliablePlace(int width, int height);
};
