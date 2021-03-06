#pragma once

#include <vector>
#include <sstream>
#include <fstream>

#include "ysglfontdata.h"
#include "fssimplewindow.h"
#include "StringPlus.h"
#include "yspng.h"
#include "DrawingUtilNG.h"

#include "Position.h"

using namespace std;

enum class pieceType {BASE, GENERAL, ADVISOR, ELEPHANT, HORSE, CHARIOT, CANNON, SOLDIER};

class Piece {
protected:
	Position* pos;
	int playerIndex;
	int pieceIndex;
	string pieceID;
	bool isCrossRiver;
	vector<vector<int>> steps;
	pieceType type;
	YsRawPngDecoder imgPIC;
	YsRawPngDecoder imgCHN;

public:
	Piece(int x, int y, int playerIndex, int pieceIndex) {
		this->pos = new Position(x, y);
		this->playerIndex = playerIndex;
		this->pieceIndex = pieceIndex;
		this->isCrossRiver = false;
		this->type = pieceType::BASE;
	}

	Position* getPos() { return pos; }
	void setPos(Position* pos) { this->pos = pos; }
	int getPlayerIndex() { return playerIndex; }
	int getPieceIndex() { return pieceIndex; }
	pieceType getType() { return type; };
	virtual vector<Position*> getAvaliablePlace(int width, int height);
	virtual bool onBoard(Position* pos);
	void setCrossRiver() {
		this->isCrossRiver = true;
	}

	bool crossRiver() {
		return this->isCrossRiver;
	}
	void addImage(std::string pathPIC, std::string pathCHN);

	//// reads data from a file over-riding any existing data
	//void load(std::ifstream& inFile);
	void draw(int mode);
};


class General : public Piece {
public:
	General(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		steps.push_back({ 1, 0 });
		steps.push_back({ -1, 0 });
		steps.push_back({ 0, 1 });
		steps.push_back({ 0, -1 });
		this->type = pieceType::GENERAL;
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
		this->type = pieceType::ADVISOR;
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
		this->type = pieceType::ELEPHANT;
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
		this->type = pieceType::HORSE;
	};
	virtual vector<Position*> getAvaliablePlace(int width, int height);
};

class Chariot : public Piece {
public:
	Chariot(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		this->type = pieceType::CHARIOT;
	};
};

class Cannon : public Piece {
public:
	Cannon(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		this->type = pieceType::CANNON;
	};
};

class Soldier : public Piece {
public:
	Soldier(int x, int y, int playerIndex, int pieceIndex) :Piece(x, y, playerIndex, pieceIndex) {
		steps.push_back({ 0, 1 });
		steps.push_back({ 1, 0 });
		steps.push_back({ -1, 0 });
		this->type = pieceType::SOLDIER;
	};
	virtual vector<Position*> getAvaliablePlace(int width, int height);
};
