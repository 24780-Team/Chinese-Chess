#include <iostream>
#include <iomanip>
#include "Board.h"

using namespace std;

#define boardMargin 50
#define gridMargin 100
#define boardSize 900
#define gridSize 100
#define pieceSize 80

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
}

Piece* Board::getPiece(shared_ptr<Position> pos)
{
	int x = pos->getX();
	int y = pos->getY();
	return posToPiece[x][y];
}

Piece* Board::setPiece(shared_ptr<Position> pos, Piece* piece)
{

	if (piece->getType() == pieceType::SOLDIER) {
		int playerIndex = piece->getPlayerIndex();
		if (playerIndex == 0) {
			if (pos->getY() > 4) piece->setCrossRiver(true);
			else piece->setCrossRiver(false);
		}

		if (playerIndex == 1) {
			if (pos->getY() < 5) piece->setCrossRiver(true);
			else piece->setCrossRiver(false);
		}
	}

	shared_ptr<Position> prevPos = piece->getPos();
	int x = pos->getX();
	int y = pos->getY();
	int prevX = prevPos->getX();
	int prevY = prevPos->getY();

	piece->setPos(pos);
	Piece* otherPiece = posToPiece[x][y];
	posToPiece[x][y] = piece;
	posToPiece[prevX][prevY] = nullptr;

	if (otherPiece == nullptr) {
		return nullptr;
	}

	int playerIndex = piece->getPlayerIndex();
	int otherPieceIndex = otherPiece->getPieceIndex();
	if (playerIndex == 0) {
		player1Alive.erase(otherPieceIndex);
		player1Dead[otherPieceIndex] = otherPiece;
	}
	else {
		player0Alive.erase(otherPieceIndex);
		player0Dead[otherPieceIndex] = otherPiece;
	}
	return otherPiece;
}

void Board::drawPieces(int mode)
{
	
	for (auto pair : player0Alive) {
		pair.second->draw(mode);
	}
	for (auto pair : player1Alive) {
		pair.second->draw(mode);
	}
}

void Board::drawCurrentFrame()
{
	if (!isChoose) {
		return;
	}
	drawChooseFrame(chooseLoc);
	return;
}

bool Board::isChooseLocationInBoard(int screenX, int screenY)
{
	if (screenX >= boardMargin && screenX <= boardMargin + boardSize
		&& screenY >= boardMargin && screenY <= 3 * boardMargin + boardSize) {
		return true;
	}
	return false;
}

void Board::changeChooseState(int screenX, int screenY)
{
	if (isChooseLocationInBoard(screenX, screenY)) {
		this->chooseLoc = getChooseLocation(screenX, screenY);
		this->isChoose = true;
	}
	else {
		isChoose = false;
		this->chooseLoc = nullptr;
	}
}

shared_ptr<Position> Board::getChooseLocation(int screenX, int screenY)
{
	int x = (screenX - boardMargin) / gridSize;
	int y = (screenY - boardMargin) / gridSize;
	return make_shared<Position>(x, y);
}

void Board::drawChooseFrame(shared_ptr<Position> theLoc)
{
	int lineWidth = 3;
	glColor3ub(0, 0, 255);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	int centerX = gridMargin + theLoc->getX() * gridSize;
	int centerY = gridMargin + theLoc->getY() * gridSize;
	int corner1X = centerX - 45;
	int corner1Y = centerY - 45;
	int corner2X = centerX + 45;
	int corner2Y = centerY - 45;
	int corner3X = centerX - 45;
	int corner3Y = centerY + 45;
	int corner4X = centerX + 45;
	int corner4Y = centerY + 45;
	glVertex2i(corner1X, corner1Y);
	glVertex2i(corner1X + 20, corner1Y);
	glVertex2i(corner1X, corner1Y);
	glVertex2i(corner1X, corner1Y + 20);

	glVertex2i(corner2X, corner2Y);
	glVertex2i(corner2X - 20, corner2Y);
	glVertex2i(corner2X, corner2Y);
	glVertex2i(corner2X, corner2Y + 20);

	glVertex2i(corner3X, corner3Y);
	glVertex2i(corner3X + 20, corner3Y);
	glVertex2i(corner3X, corner3Y);
	glVertex2i(corner3X, corner3Y - 20);

	glVertex2i(corner4X, corner4Y);
	glVertex2i(corner4X - 20, corner4Y);
	glVertex2i(corner4X, corner4Y);
	glVertex2i(corner4X, corner4Y - 20);

	glEnd();
	glFlush();
}

void Board::drawModeChooseFrame()
{
	glColor3ub(238, 197, 145);
	glBegin(GL_QUADS);
	int x1 = gridMargin + 9.5 * gridSize;
	int x2 = gridMargin + 12.5 * gridSize;
	int y1 = boardMargin + 4 * gridSize;
	int y2 = boardMargin + 4.5 * gridSize;
	glVertex2i(x1, y1);
	glVertex2i(x1, y2);
	glVertex2i(x2, y2);
	glVertex2i(x2, y1);
	glEnd();
	glFlush();

	glColor3ub(0, 0, 0);
	glRasterPos2i(x1 + pieceSize / 8, y1 + pieceSize / 2);
	string p1 = "change pattern";
	const char* h1 = p1.c_str();
	YsGlDrawFontBitmap20x32(h1);
	glFlush();
}

void Board::drawPlayerFrame(int currPlayerIndex)
{
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	int p0x1 = gridMargin + 9.5 * gridSize;
	int p0x2 = gridMargin + 11 * gridSize;
	int p0y1 = boardMargin + 2 * gridSize;
	int p0y2 = boardMargin + 2.5 * gridSize;

	int p1x1 = gridMargin + 9.5 * gridSize;
	int p1x2 = gridMargin + 11 * gridSize;
	int p1y1 = boardMargin + 6 * gridSize;
	int p1y2 = boardMargin + 6.5 * gridSize;
	
	if (currPlayerIndex == 0) {
		glVertex2i(p0x1, p0y1);
		glVertex2i(p0x1, p0y2);
		glVertex2i(p0x2, p0y2);
		glVertex2i(p0x2, p0y1);
	}
	else {
		glVertex2i(p1x1, p1y1);
		glVertex2i(p1x1, p1y2);
		glVertex2i(p1x2, p1y2);
		glVertex2i(p1x2, p1y1);
	}
	
	glEnd();
	glFlush();

	glColor3ub(255, 0, 0);
	glRasterPos2i(p0x1 + pieceSize / 8, p0y1 + pieceSize / 2);
	string p1 = "Player1";
	const char* h1 = p1.c_str();
	YsGlDrawFontBitmap20x32(h1);

	glColor3ub(0, 0, 0);
	glRasterPos2i(p1x1 + pieceSize / 8, p1y1 + pieceSize / 2);
	string p2 = "Player2";
	const char* h2 = p2.c_str();
	YsGlDrawFontBitmap20x32(h2);
	glFlush();
}

void Board::drawNodes(const vector<shared_ptr<Position>>& avaliablePlaces)
{
	const double PI = 3.1415927;
	for (auto pos : avaliablePlaces) {
		glBegin(GL_POLYGON);
		glColor3ub(rgbr, rgbg, rgbb);
		int centerX = gridMargin + pos->getX() * gridSize;
		int centerY = gridMargin + pos->getY() * gridSize;
		for (int i = 0; i < 64; i++) {
			double angle = (double)i * PI / 32.0;
			double x = (double)centerX + cos(angle) * (double)15;
			double y = (double)centerY + sin(angle) * (double)15;
			glVertex2d(x, y);
		}
		glEnd();
		glFlush();
	}
}

bool Board::isChooseLocationInChangePattern(int screenX, int screenY)
{
	if (screenX >= gridMargin + 9.5 * gridSize && screenX <= gridMargin + 12.5 * gridSize
		&& screenY >= boardMargin + 4 * gridSize && screenY <= boardMargin + 4.5 * gridSize) {
		return true;
	}
	return false;
}

void Board::setAlive(Piece* piece)
{
	shared_ptr<Position> pos = piece->getPos();
	int x = pos->getX();
	int y = pos->getY();
	posToPiece[x][y] = piece;
	int playerIndex = piece->getPlayerIndex();
	int pieceIndex = piece->getPieceIndex();
	if (playerIndex == 1) {
		player1Dead.erase(pieceIndex);
		player1Alive[pieceIndex] = piece;
	}
	else {
		player0Dead.erase(pieceIndex);
		player0Alive[pieceIndex] = piece;
	}
}

int Board::calcScore(int playerIndex)
{	
	int score = 0; 
	unordered_map<int, Piece*> deadPieces;
	if (playerIndex == 0) {
		deadPieces = player0Dead;
	}
	else {
		deadPieces = player1Dead;
	}
	
	for (auto pair : deadPieces) {
		Piece* piece = pair.second;
		switch (piece->getType()) {
		case pieceType::SOLDIER:
			score += 1;
			break;
		case pieceType::ADVISOR:
		case pieceType::ELEPHANT:
			score += 2;
			break;
		case pieceType::HORSE:
			score += 3;
			break;
		case pieceType::CANNON:
			score += 4;
			break;
		case pieceType::CHARIOT:
			score += 6;
			break;
		case pieceType::GENERAL:
			score += 10;
			break;
		}
	}
	return score;
}

void Board::getAvaliblePlaces(Piece* piece, vector<shared_ptr<Position>>& places)
{
	vector<shared_ptr<Position>>().swap(places);
	vector<shared_ptr<Position>> positions = piece->getAvaliablePlace(width, height);
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
	case pieceType::GENERAL:
		positions = checkForGeneral(piece, positions);
		break;
	}

	for (auto pos : positions) {
		int x = pos->getX();
		int y = pos->getY();
		Piece* otherPiece = posToPiece[x][y];

		if (otherPiece != nullptr && otherPiece->getPlayerIndex() == currPlayerIndex) {
			continue;
		}

		places.push_back(pos);
	}
}

Piece* Board::getAlivePieceByIndex(int index)
{
	if (player0Alive.count(index) != 0) {
		return player0Alive[index];
	}
	
	if (player1Alive.count(index) != 0) {
		return player1Alive[index];
	}
	return nullptr;
}

Piece* Board::getDeadPieceByIndex(int index)
{
	if (player0Alive.count(index) != 0) {
		return player0Dead[index];
	}

	if (player1Alive.count(index) != 0) {
		return player1Dead[index];
	}
	return nullptr;
}

bool Board::lossGeneral(int playerIndex)
{
	if (playerIndex == 0) {
		if (player0Dead.find(general0Index) != player0Dead.end())
			return true;
		return false;
	}
	else {
		if (player1Dead.find(general1Index) != player1Dead.end())
			return true;
		return false;
	}
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

void Board::getPlacesOfPieces(int playerIndex, vector<shared_ptr<Position>>& places)
{
	vector<shared_ptr<Position>>().swap(places);
	if (playerIndex == 0) {
		for (auto v : player0Alive) {
			places.push_back(v.second->getPos());
		}
	}
	else {
		for (auto v : player1Alive) {
			places.push_back(v.second->getPos());
		}
	}
}

void Board::initializeGeneral(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	pos = make_shared<Position>(4, 0);
	piece = new General(4, 0, 0, index);
	piece->addImage("Resources/pieces/red_pic/general.png", "Resources/pieces/red_cn/general.png");

	player0Alive[index] = piece;
	posToPiece[4][0] = piece;
	index += 1;
	
	pos = make_shared<Position>(4, 9);
	piece = new General(4, 9, 1, index);
	piece->addImage("Resources/pieces/black_pic/general.png", "Resources/pieces/black_cn/general.png");

	player1Alive[index] = piece;
	posToPiece[4][9] = piece;
	index += 1;

	general0Index = 0;
	general1Index = 1;
}

void Board::initializeAdvisor(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -1; i <= 1; i += 2) {
		pos = make_shared<Position>(4 + i, 0);
		piece = new Advisor(4 + i, 0, 0, index);
		piece->addImage("Resources/pieces/red_pic/advisor.png", "Resources/pieces/red_cn/advisor.png");

		player0Alive[index] = piece;
		posToPiece[4 + i][0] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 9);
		piece = new Advisor(4 + i, 9, 1, index);
		piece->addImage("Resources/pieces/black_pic/advisor.png", "Resources/pieces/black_cn/advisor.png");

		player1Alive[index] = piece;
		posToPiece[4 + i][9] = piece;
		index += 1;
	}
}

void Board::initializeElephant(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -2; i <= 2; i += 4) {
		pos = make_shared<Position>(4 + i, 0);
		piece = new Elephant(4 + i, 0, 0, index);
		piece->addImage("Resources/pieces/red_pic/elephant.png", "Resources/pieces/red_cn/elephant.png");

		player0Alive[index] = piece;
		posToPiece[4 + i][0] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 9);
		piece = new Elephant(4 + i, 9, 1, index);
		piece->addImage("Resources/pieces/black_pic/elephant.png", "Resources/pieces/black_cn/elephant.png");

		player1Alive[index] = piece;
		posToPiece[4 + i][9] = piece;
		index += 1;
	}
}

void Board::initializeHorse(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -3; i <= 3; i += 6) {
		pos = make_shared<Position>(4 + i, 0);
		piece = new Horse(4 + i, 0, 0, index);
		piece->addImage("Resources/pieces/red_pic/horse.png", "Resources/pieces/red_cn/horse.png");
		
		player0Alive[index] = piece;
		posToPiece[4 + i][0] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 9);
		piece = new Horse(4 + i, 9, 1, index);
		piece->addImage("Resources/pieces/black_pic/horse.png", "Resources/pieces/black_cn/horse.png");
		
		player1Alive[index] = piece;
		posToPiece[4 + i][9] = piece;
		index += 1;
	}
}

void Board::initializeChariot(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -4; i <= 4; i += 8) {
		pos = make_shared<Position>(4 + i, 0);
		piece = new Chariot(4 + i, 0, 0, index);
		piece->addImage("Resources/pieces/red_pic/chariot.png", "Resources/pieces/red_cn/chariot.png");

		player0Alive[index] = piece;
		posToPiece[4 + i][0] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 9);
		piece = new Chariot(4 + i, 9, 1, index);
		piece->addImage("Resources/pieces/black_pic/chariot.png", "Resources/pieces/black_cn/chariot.png");
		
		player1Alive[index] = piece;
		posToPiece[4 + i][9] = piece;
		index += 1;
	}
}

void Board::initializeCannon(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -3; i <= 3; i += 6) {
		pos = make_shared<Position>(4 + i, 2);
		piece = new Cannon(4 + i, 2, 0, index);
		piece->addImage("Resources/pieces/red_pic/cannon.png", "Resources/pieces/red_cn/cannon.png");
		
		player0Alive[index] = piece;
		posToPiece[4 + i][2] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 7);
		piece = new Cannon(4 + i, 7, 1, index);
		piece->addImage("Resources/pieces/black_pic/cannon.png", "Resources/pieces/black_cn/cannon.png");
		
		player1Alive[index] = piece;
		posToPiece[4 + i][7] = piece;
		index += 1;
	}
}

void Board::initializeSoldier(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = 0; i < 9; i += 2) {
		pos = make_shared<Position>(i, 3);
		piece = new Soldier(i, 3, 0, index);
		piece->addImage("Resources/pieces/red_pic/soldier.png", "Resources/pieces/red_cn/soldier.png");
		
		player0Alive[index] = piece;
		posToPiece[i][3] = piece;
		index += 1;

		pos = make_shared<Position>(i, 6);
		piece = new Soldier(i, 6, 1, index);
		piece->addImage("Resources/pieces/black_pic/soldier.png", "Resources/pieces/black_cn/soldier.png");
		
		player1Alive[index] = piece;
		posToPiece[i][6] = piece;
		index += 1;
	}
}

vector<shared_ptr<Position>> Board::checkForElephant(Piece* piece, vector<shared_ptr<Position>>& positions)
{
	int currX = piece->getPos()->getX();
	int currY = piece->getPos()->getY();
	
	for (auto iter = positions.begin(); iter != positions.end();) {
		shared_ptr<Position> pos = *iter;
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

vector<shared_ptr<Position>> Board::checkForHorse(Piece* piece, vector<shared_ptr<Position>>& positions)
{
	int currX = piece->getPos()->getX();
	int currY = piece->getPos()->getY();

	for (auto iter = positions.begin(); iter != positions.end();) {
		shared_ptr<Position> pos = *iter;
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

std::vector<shared_ptr<Position>> Board::checkForGeneral(Piece* piece, std::vector<shared_ptr<Position>>& positions)
{
	int x = piece->getPos()->getX();
	int y = piece->getPos()->getY();
	int playerIndex = piece->getPlayerIndex();
	if (playerIndex == 0) {
		for (int j = y + 1; j < height; j++) {
			if (posToPiece[x][j] != nullptr) {
				Piece* piece = posToPiece[x][j];
				if (piece->getType() == pieceType::GENERAL) {
					shared_ptr<Position> pos(new Position(x, j));
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
					shared_ptr<Position> pos(new Position(x, j));
					positions.push_back(pos);
				}
				break;
			}
		}
	}
	return positions;
}

vector<shared_ptr<Position>> Board::checkForChariot(Piece* piece)
{
	vector<shared_ptr<Position>> avaliablePlace;
	int x = piece->getPos()->getX();
	int y = piece->getPos()->getY();
	int playerIndex = piece->getPlayerIndex();
	for (int i = x + 1; i < width; i++) {
		shared_ptr<Position> pos;
		pos = make_shared<Position>(i, y);
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
		shared_ptr<Position> pos;
		pos = make_shared<Position>(i, y);
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
		shared_ptr<Position> pos;
		pos = make_shared<Position>(x, j);
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
		shared_ptr<Position> pos;
		pos = make_shared<Position>(x, j);
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

vector<shared_ptr<Position>> Board::checkForCannon(Piece* piece)
{
	vector<shared_ptr<Position>> avaliablePlace;
	int x = piece->getPos()->getX();
	int y = piece->getPos()->getY();
	int playerIndex = piece->getPlayerIndex();
	bool meetObstacle = false;

	for (int i = x + 1; i < width; i++) {
		shared_ptr<Position> pos;
		pos = make_shared<Position>(i, y);
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
		shared_ptr<Position> pos;
		pos = make_shared<Position>(i, y);
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
		shared_ptr<Position> pos;
		pos = make_shared<Position>(x, j);
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
		shared_ptr<Position> pos;
		pos = make_shared<Position>(x, j);
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

void Board::drawBoard()
{
	// board background
	glColor3ub(238, 197, 145);
	glBegin(GL_QUADS);
	glVertex2i(boardMargin, boardMargin);
	glVertex2i(boardMargin, 3 * boardMargin + boardSize);
	glVertex2i(boardMargin + boardSize, 3 * boardMargin + boardSize);
	glVertex2i(boardMargin + boardSize, boardMargin);
	glEnd();
	glFlush();

	// board grid
	int lineWidth = 3;
	// horizontal lines
	glColor3ub(0, 0, 0);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	for (int i = 0; i <= 9; i++) {
		int x1 = gridMargin;
		int y1 = gridMargin + gridSize * i;
		int x2 = gridMargin + gridSize * 8;
		int y2 = gridMargin + gridSize * i;
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	}
	glEnd();
	glFlush();

	// vertical lines
	glColor3ub(0, 0, 0);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	for (int i = 0; i <= 8; i++) {
		int x = gridMargin + gridSize * i;
		int y1 = gridMargin;
		int y2 = gridMargin + gridSize * 4;
		int y3 = gridMargin + gridSize * 5;
		int y4 = gridMargin + gridSize * 9;
		if (i == 0 || i == 8) {
			glVertex2i(x, y1);
			glVertex2i(x, y4);
		}
		else {
			glVertex2i(x, y1);
			glVertex2i(x, y2);
			glVertex2i(x, y3);
			glVertex2i(x, y4);
		}
	}
	glEnd();
	glFlush();

	// diagonal lines
	glColor3ub(0, 0, 0);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	int x1 = gridMargin + gridSize * 3;
	int y1 = gridMargin;
	int x2 = gridMargin + gridSize * 5;
	int y2 = gridMargin + gridSize * 2;
	int x3 = gridMargin + gridSize * 5;
	int y3 = gridMargin;
	int x4 = gridMargin + gridSize * 3;
	int y4 = gridMargin + gridSize * 2;
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glVertex2i(x4, y4);
	glVertex2i(x1, y1 + 7 * gridSize);
	glVertex2i(x2, y2 + 7 * gridSize);
	glVertex2i(x3, y3 + 7 * gridSize);
	glVertex2i(x4, y4 + 7 * gridSize);
	glEnd();
	glFlush();

	// river 
	glRasterPos2i(gridMargin + 3 * gridSize, gridMargin + 4.6 * gridSize);
	glColor3ub(0, 0, 0);
	string header = "The river";
	const char* h = header.c_str();
	YsGlDrawFontBitmap20x32(h);
	glFlush();
}

//void Board::addPiece(const string filename, int playerIndex, int startIndex)
//{
//	ifstream inFile;
//	string datafilename;
//	int index = startIndex;
//
//	datafilename = filename;
//	if (datafilename.find(".txt") == string::npos)
//		datafilename += ".txt";     // allows user to omit extension
//	inFile.open(datafilename);
//
//	if (inFile.is_open()) { // may not have found file
//		string wholeLineString;
//
//		while (!inFile.eof()) {
//			// read the whole line
//			getline(inFile, wholeLineString);
//			if (wholeLineString.find("Piece:") != string::npos) {
//				Piece* newPiece = new Piece(0, 0, playerIndex, index);
//				newPiece->load(inFile);
//				if (playerIndex == 0)
//					player0Alive[index] = (newPiece);
//				else
//					player0Alive[index] = (newPiece);
//			}
//			index += 1;
//		}
//		inFile.close();
//	}
//	else
//		cout << "\nError reading file. Please check data and try again." << endl;
//}