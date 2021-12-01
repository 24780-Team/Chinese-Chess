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

	const char* texture1Path = "Resources/background/texture.png";
	if (YSOK == backgroundTexture1.Decode(texture1Path)) {
		backgroundTexture1.Flip();
	}

	const char* texture2Path = "Resources/background/texture2.png";
	if (YSOK == backgroundTexture2.Decode(texture2Path)) {
		backgroundTexture2.Flip();
	}

	const char* playerInformationBoardPath = "Resources/background/board.png";
	if (YSOK == playerInformBoard.Decode(playerInformationBoardPath)) {
		playerInformBoard.Flip();
	}

	const char* backgroundColorButtonPath = "Resources/buttons/background.png";
	if (YSOK == backgroundColorButton.Decode(backgroundColorButtonPath)) {
		backgroundColorButton.Flip();
	}

	const char* backgroundPicButtonPath = "Resources/buttons/picture.png";
	if (YSOK == backgroundPicButton.Decode(backgroundPicButtonPath)) {
		backgroundPicButton.Flip();
	}

	const char* musicOnButtonPath = "Resources/buttons/musicon.png";
	if (YSOK == musicOnButton.Decode(musicOnButtonPath)) {
		musicOnButton.Flip();
	}

	const char* musicOffButtonPath = "Resources/buttons/musicoff.png";
	if (YSOK == musicOffButton.Decode(musicOffButtonPath)) {
		musicOffButton.Flip();
	}

	const char* piecePatternButtonPath = "Resources/buttons/piece.png";
	if (YSOK == piecePatternButton.Decode(piecePatternButtonPath)) {
		piecePatternButton.Flip();
	}

	const char* surrenderTextPath = "Resources/words/surrender.png";
	if (YSOK == surrenderText.Decode(surrenderTextPath)) {
		surrenderText.Flip();
	}

	const char* retractTextPath = "Resources/words/retract.png";
	if (YSOK == retractText.Decode(retractTextPath)) {
		retractText.Flip();
	}

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
		if (playerIndex == 1) {
			if (pos->getY() > 4) piece->setCrossRiver(true);
			else piece->setCrossRiver(false);
		}

		if (playerIndex == 0) {
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


void Board::drawPlayerFrame(int currPlayerIndex, string nameOfP0, string nameOfP1)
{
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	int p0x1 = gridMargin + 9.2 * gridSize;
	int p0x2 = gridMargin + 12.2 * gridSize;
	int p0y1 = gridMargin + 8 * gridSize;
	int p0y2 = gridMargin + 5.5 * gridSize;

	int p1x1 = gridMargin + 9.2 * gridSize;
	int p1x2 = gridMargin + 12.2 * gridSize;
	int p1y1 = gridMargin + 4 * gridSize;
	int p1y2 = gridMargin + 1.5 * gridSize;

	if (currPlayerIndex == 0) {
		// Frame
		glVertex2i(p0x1, p0y1);
		glVertex2i(p0x1, p0y2);
		glVertex2i(p0x2, p0y2);
		glVertex2i(p0x2, p0y1);
		// General
		// TODO
		glRasterPos2i(gridMargin + 10.2 * gridSize, gridMargin + 6 * gridSize);
		glDrawPixels(player0Alive[general0Index]->getImage(piecesPattern)->wid, player0Alive[general0Index]->getImage(piecesPattern)->hei, GL_RGBA, GL_UNSIGNED_BYTE, player0Alive[general0Index]->getImage(piecesPattern)->rgba);
		
	}
	else {
		// Frame
		glVertex2i(p1x1, p1y1);
		glVertex2i(p1x1, p1y2);
		glVertex2i(p1x2, p1y2);
		glVertex2i(p1x2, p1y1);
		// General

	}

	glEnd();
	glFlush();

	glColor3ub(255, 0, 0);
	glRasterPos2i(p0x1 + pieceSize / 8, p0y2 + pieceSize / 2 + 10);
	YsGlDrawFontBitmap20x32(nameOfP0.c_str());

	glColor3ub(0, 0, 0);
	glRasterPos2i(p1x1 + pieceSize / 8, p1y2 + pieceSize / 2 + 10);
	YsGlDrawFontBitmap20x32(nameOfP1.c_str());
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

void Board::drawPlayerInformation()
{
	// Info background texture
	// Player 1
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glRasterPos2i(gridMargin + 9.2 * gridSize, gridMargin + 8 * gridSize);
	glDrawPixels(playerInformBoard.wid, playerInformBoard.hei, GL_RGBA, GL_UNSIGNED_BYTE, playerInformBoard.rgba);
	// time
	
	// surrender button
	glRasterPos2i(gridMargin + 10.2 * gridSize, gridMargin + 8.4 * gridSize);
	glDrawPixels(surrenderText.wid, surrenderText.hei, GL_RGBA, GL_UNSIGNED_BYTE, surrenderText.rgba);	
	// retract button
	glRasterPos2i(gridMargin + 9 * gridSize, gridMargin + 8.4 * gridSize);
	glDrawPixels(retractText.wid, retractText.hei, GL_RGBA, GL_UNSIGNED_BYTE, retractText.rgba);

	// Player 2
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glRasterPos2i(gridMargin + 9.2 * gridSize, gridMargin + 4 * gridSize);
	glDrawPixels(playerInformBoard.wid, playerInformBoard.hei, GL_RGBA, GL_UNSIGNED_BYTE, playerInformBoard.rgba);
	// time

	// surrender button
	glRasterPos2i(gridMargin + 10.2 * gridSize, gridMargin + 4.4 * gridSize);
	glDrawPixels(surrenderText.wid, surrenderText.hei, GL_RGBA, GL_UNSIGNED_BYTE, surrenderText.rgba);
	// retract button
	glRasterPos2i(gridMargin + 9 * gridSize, gridMargin + 4.4 * gridSize);
	glDrawPixels(retractText.wid, retractText.hei, GL_RGBA, GL_UNSIGNED_BYTE, retractText.rgba);

	
}

void Board::drawBoard()
{
	// Board color
	setBackgroundColor();
	glBegin(GL_QUADS);
	glVertex2i(boardMargin, boardMargin);
	glVertex2i(boardMargin, 3 * boardMargin + boardSize);
	glVertex2i(boardMargin + boardSize, 3 * boardMargin + boardSize);
	glVertex2i(boardMargin + boardSize, boardMargin);
	glEnd();
	glFlush();

	// Board picture
	setBackgroundPic();

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

// #6 Background
void Board::setBackgroundColor()
{
	switch (backgroundColor) {
		// 
	case 1:
		glColor3ub(238, 197, 145);
		break;
	case 2:
		// Cornsilk
		glColor3ub(255, 248, 220);
		break;
	case 3:
		// GhostWhite
		glColor3ub(248, 248, 255);
		break;

	}
}

// #6 Background
void Board::setBackgroundPic()
{
	if (backgroundPic == 0) {
		// do nothing
		;
	}
	else {
		if (backgroundPic == 1) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glRasterPos2i(boardMargin, boardMargin + 10 * gridSize);
			glDrawPixels(backgroundTexture1.wid, backgroundTexture1.hei, GL_RGBA, GL_UNSIGNED_BYTE, backgroundTexture1.rgba);
		}

		else if (backgroundPic == 2) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glRasterPos2i(boardMargin, boardMargin + 10 * gridSize);
			glDrawPixels(backgroundTexture2.wid, backgroundTexture2.hei, GL_RGBA, GL_UNSIGNED_BYTE, backgroundTexture2.rgba);
		}
	}
}

// #6 Background
void Board::changeBackgroundColor()
{
	backgroundColor++;
	if (backgroundColor > 3)
		backgroundColor = 1;
}

// #6 Background
void Board::changeBackgroundPic()
{
	backgroundPic++;
	if (backgroundPic > 2)
		backgroundPic = 0;
}

// #6 Background
void Board::drawButtons()
{
	//setBackgroundColor();
	glColor3ub(0, 0, 0);
	int iconSize = 50;
	int x11 = gridMargin + 9.2 * gridSize;
	int y11 = boardMargin + 0 * gridSize;
	int x12 = gridMargin + 9.2 * gridSize + iconSize;
	int y12 = boardMargin + 0 * gridSize + iconSize;
	// draw for icon boarder
	glBegin(GL_LINE_LOOP);
	glVertex2i(x11, y11);
	glVertex2i(x11, y12);
	glVertex2i(x12, y12);
	glVertex2i(x12, y11);
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glRasterPos2i(x11 + 5, y11 + iconSize - 5);
	glDrawPixels(backgroundColorButton.wid, backgroundColorButton.hei, GL_RGBA, GL_UNSIGNED_BYTE, backgroundColorButton.rgba);


	int x21 = x11 + 0.8 * gridSize;
	int y21 = boardMargin + 0 * gridSize;
	int x22 = x11 + 0.8 * gridSize + iconSize;
	int y22 = boardMargin + 0 * gridSize + iconSize;
	// draw for icon boarder
	glBegin(GL_LINE_LOOP);
	glVertex2i(x21, y21);
	glVertex2i(x21, y22);
	glVertex2i(x22, y22);
	glVertex2i(x22, y21);
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glRasterPos2i(x21 + 5, y21 + iconSize - 5);
	glDrawPixels(backgroundPicButton.wid, backgroundPicButton.hei, GL_RGBA, GL_UNSIGNED_BYTE, backgroundPicButton.rgba);


	int x31 = x21 + 0.8 * gridSize;
	int y31 = boardMargin + 0 * gridSize;
	int x32 = x21 + 0.8 * gridSize + iconSize;
	int y32 = boardMargin + 0 * gridSize + iconSize;
	// draw for icon boarder
	glBegin(GL_LINE_LOOP);
	glVertex2i(x31, y31);
	glVertex2i(x31, y32);
	glVertex2i(x32, y32);
	glVertex2i(x32, y31);
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glRasterPos2i(x31 + 5, y31 + iconSize - 5);
	glDrawPixels(piecePatternButton.wid, piecePatternButton.hei, GL_RGBA, GL_UNSIGNED_BYTE, piecePatternButton.rgba);


	int x41 = x31 + 0.8 * gridSize;
	int y41 = boardMargin + 0 * gridSize;
	int x42 = x31 + 0.8 * gridSize + iconSize;
	int y42 = boardMargin + 0 * gridSize + iconSize;
	// draw for icon boarder
	glBegin(GL_LINE_LOOP);
	glVertex2i(x41, y41);
	glVertex2i(x41, y42);
	glVertex2i(x42, y42);
	glVertex2i(x42, y41);
	glEnd();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glRasterPos2i(x41 + 5, y41 + iconSize - 5);
	if (music_on) {
		glDrawPixels(musicOnButton.wid, musicOnButton.hei, GL_RGBA, GL_UNSIGNED_BYTE, musicOnButton.rgba);
	}
	else {
		glDrawPixels(musicOffButton.wid, musicOffButton.hei, GL_RGBA, GL_UNSIGNED_BYTE, musicOffButton.rgba);
	}

}


int Board::isInButtons(int screenX, int screenY)
{
	int iconSize = 50;

	int x11 = gridMargin + 9.2 * gridSize;
	int y11 = boardMargin;
	int x12 = gridMargin + 9.2 * gridSize + iconSize;
	int y12 = boardMargin + iconSize;
	int x21 = x11 + 0.8 * gridSize;
	int y21 = y11;
	int x22 = x11 + 0.8 * gridSize + iconSize;
	int y22 = y11 + iconSize;
	int x31 = x21 + 0.8 * gridSize;
	int y31 = y11;
	int x32 = x21 + 0.8 * gridSize + iconSize;
	int y32 = y11 + iconSize;
	int x41 = x31 + 0.8 * gridSize;
	int y41 = y11;
	int x42 = x31 + 0.8 * gridSize + iconSize;
	int y42 = y11 + iconSize;
	int x51 = gridMargin + 9.5 * gridSize;
	int y51 = gridMargin + 7.6 * gridSize;
	int x52 = x51 + 80;
	int y52 = y51 + 40;
	int x53 = gridMargin + 9.5 * gridSize;
	int y53 = gridMargin + 3.6 * gridSize;
	int x54 = x53 + 130;
	int y54 = y53 + 40;
	int x61 = gridMargin + 10.5 * gridSize;
	int y61 = gridMargin + 7.6 * gridSize;
	int x62 = x61 + 80;
	int y62 = y61 + 40;
	int x63 = gridMargin + 10.5 * gridSize;
	int y63 = gridMargin + 3.6 * gridSize;
	int x64 = x63 + 130;
	int y64 = y63 + 40;

	if (screenX >= x11 && screenX <= x12 && screenY >= y11 && screenY <= y12)
		return 1;
	if (screenX >= x21 && screenX <= x22 && screenY >= y21 && screenY <= y22)
		return 2;
	if (screenX >= x31 && screenX <= x32 && screenY >= y31 && screenY <= y32)
		return 3;
	if (screenX >= x41 && screenX <= x42 && screenY >= y41 && screenY <= y42)
		return 4;
	if (screenX >= x51 && screenX <= x52 && screenY >= y51 && screenY <= y52)
		return 5;
	if (screenX >= x53 && screenX <= x54 && screenY >= y53 && screenY <= y54)
		return 5;
	if (screenX >= x61 && screenX <= x62 && screenY >= y61 && screenY <= y62)
		return 6;
	if (screenX >= x63 && screenX <= x64 && screenY >= y63 && screenY <= y64)
		return 6;

	return -1;
}

// #6 Background 
void Board::drawPieces()
{
	for (auto pair : player0Alive) {
		pair.second->draw(piecesPattern);
	}
	for (auto pair : player1Alive) {
		pair.second->draw(piecesPattern);
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
	if (playerIndex == 1) {
		if (player1Dead.find(general1Index) != player1Dead.end())
			return true;
		return false;
	}
	else {
		if (player0Dead.find(general0Index) != player0Dead.end())
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

	pos = make_shared<Position>(4, 9);
	piece = new General(4, 9, 0, index);
	piece->addImage("Resources/pieces/red_pic/general.png", "Resources/pieces/red_cn/general.png");

	player0Alive[index] = piece;
	posToPiece[4][9] = piece;
	index += 1;

	pos = make_shared<Position>(4, 0);
	piece = new General(4, 0, 1, index);
	piece->addImage("Resources/pieces/black_pic/general.png", "Resources/pieces/black_cn/general.png");

	player1Alive[index] = piece;
	posToPiece[4][0] = piece;
	index += 1;

	general0Index = 0;
	general1Index = 1;
}

void Board::initializeAdvisor(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -1; i <= 1; i += 2) {
		pos = make_shared<Position>(4 + i, 9);
		piece = new Advisor(4 + i, 9, 0, index);
		piece->addImage("Resources/pieces/red_pic/advisor.png", "Resources/pieces/red_cn/advisor.png");

		player0Alive[index] = piece;
		posToPiece[4 + i][9] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 0);
		piece = new Advisor(4 + i, 0, 1, index);
		piece->addImage("Resources/pieces/black_pic/advisor.png", "Resources/pieces/black_cn/advisor.png");

		player1Alive[index] = piece;
		posToPiece[4 + i][0] = piece;
		index += 1;
	}
}

void Board::initializeElephant(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -2; i <= 2; i += 4) {
		pos = make_shared<Position>(4 + i, 9);
		piece = new Elephant(4 + i, 9, 0, index);
		piece->addImage("Resources/pieces/red_pic/elephant.png", "Resources/pieces/red_cn/elephant.png");

		player0Alive[index] = piece;
		posToPiece[4 + i][9] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 0);
		piece = new Elephant(4 + i, 0, 1, index);
		piece->addImage("Resources/pieces/black_pic/elephant.png", "Resources/pieces/black_cn/elephant.png");

		player1Alive[index] = piece;
		posToPiece[4 + i][0] = piece;
		index += 1;
	}
}

void Board::initializeHorse(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -3; i <= 3; i += 6) {
		pos = make_shared<Position>(4 + i, 9);
		piece = new Horse(4 + i, 9, 0, index);
		piece->addImage("Resources/pieces/red_pic/horse.png", "Resources/pieces/red_cn/horse.png");

		player0Alive[index] = piece;
		posToPiece[4 + i][9] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 0);
		piece = new Horse(4 + i, 0, 1, index);
		piece->addImage("Resources/pieces/black_pic/horse.png", "Resources/pieces/black_cn/horse.png");

		player1Alive[index] = piece;
		posToPiece[4 + i][0] = piece;
		index += 1;
	}
}

void Board::initializeChariot(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -4; i <= 4; i += 8) {
		pos = make_shared<Position>(4 + i, 9);
		piece = new Chariot(4 + i, 9, 0, index);
		piece->addImage("Resources/pieces/red_pic/chariot.png", "Resources/pieces/red_cn/chariot.png");

		player0Alive[index] = piece;
		posToPiece[4 + i][9] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 0);
		piece = new Chariot(4 + i, 0, 1, index);
		piece->addImage("Resources/pieces/black_pic/chariot.png", "Resources/pieces/black_cn/chariot.png");

		player1Alive[index] = piece;
		posToPiece[4 + i][0] = piece;
		index += 1;
	}
}

void Board::initializeCannon(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = -3; i <= 3; i += 6) {
		pos = make_shared<Position>(4 + i, 7);
		piece = new Cannon(4 + i, 7, 0, index);
		piece->addImage("Resources/pieces/red_pic/cannon.png", "Resources/pieces/red_cn/cannon.png");

		player0Alive[index] = piece;
		posToPiece[4 + i][7] = piece;
		index += 1;

		pos = make_shared<Position>(4 + i, 2);
		piece = new Cannon(4 + i, 2, 1, index);
		piece->addImage("Resources/pieces/black_pic/cannon.png", "Resources/pieces/black_cn/cannon.png");

		player1Alive[index] = piece;
		posToPiece[4 + i][2] = piece;
		index += 1;
	}
}

void Board::initializeSoldier(int& index)
{
	shared_ptr<Position> pos;
	Piece* piece;

	for (int i = 0; i < 9; i += 2) {
		pos = make_shared<Position>(i, 6);
		piece = new Soldier(i, 6, 0, index);
		piece->addImage("Resources/pieces/red_pic/soldier.png", "Resources/pieces/red_cn/soldier.png");

		player0Alive[index] = piece;
		posToPiece[i][6] = piece;
		index += 1;

		pos = make_shared<Position>(i, 3);
		piece = new Soldier(i, 3, 1, index);
		piece->addImage("Resources/pieces/black_pic/soldier.png", "Resources/pieces/black_cn/soldier.png");

		player1Alive[index] = piece;
		posToPiece[i][3] = piece;
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
	if (playerIndex == 1) {
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
