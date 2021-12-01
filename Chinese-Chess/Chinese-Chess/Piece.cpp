#include <vector>

#include "StringPlus.h"

#include "Position.h"
#include "Piece.h"

#define boardMargin 50
#define gridMargin 100
#define boardSize 900
#define gridSize 100
#define pieceSize 80

vector<shared_ptr<Position>> Piece::getAvaliablePlace(int width, int height)
{
	return vector<shared_ptr<Position>>();
}

bool Piece::onBoard(shared_ptr<Position> pos)
{
	if (pos->getX() < 0 || pos->getX() > 8 || pos->getY() < 0 || pos->getY() > 9) {
		return false;
	}
	else {
		return true;
	}
}

void Piece::addImage(string pathPIC, string pathCHN)
{
	imgPIC.Decode(pathPIC.c_str());
	imgCHN.Decode(pathCHN.c_str());
	imgPIC.Flip();
	imgCHN.Flip();
}

void Piece::draw(int mode)
{
	int x = pos->getX();
	int y = pos->getY();
	YsRawPngDecoder *png;
	if (mode == 0) {
		png = &imgPIC;
	}
	else {
		png = &imgCHN;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glRasterPos2i(gridMargin + gridSize * x - gridSize * 0.4, gridMargin + gridSize * y + pieceSize / 2);
	glDrawPixels(png->wid, png->hei, GL_RGBA, GL_UNSIGNED_BYTE, png->rgba);
}

YsRawPngDecoder * Piece::getImage(int piecePattern)
{
	if (piecePattern == 0) {
		return &imgCHN;
	}
	else {
		return &imgPIC;
	}
}


vector<shared_ptr<Position>> General::getAvaliablePlace(int width, int height)
{
	vector<shared_ptr<Position>> avaliablePlace;
	for (auto step : steps) {
		shared_ptr<Position> dest(new Position(pos->getX() + step[0], pos->getY() + step[1]));
		if (!onBoard(dest)) {
			continue;
		}
		avaliablePlace.push_back(dest);
	}
	return avaliablePlace;
}

bool General::onBoard(shared_ptr<Position> pos)
{
	if (playerIndex == 1) {
		if (pos->getX() < 3 || pos->getX() > 5 || pos->getY() < 0 || pos->getY() > 2) {
			return false;
		} 
	}
	else {
		if (pos->getX() < 3 || pos->getX() > 5 || pos->getY() < 7 || pos->getY() > 9) {
			return false;
		}
	}
	return true;
}

bool Advisor::onBoard(shared_ptr<Position> pos)
{
	if (playerIndex == 1) {
		if (pos->getX() < 3 || pos->getX() > 5 || pos->getY() < 0 || pos->getY() > 2) {
			return false;
		}
	}
	else {
		if (pos->getX() < 3 || pos->getX() > 5 || pos->getY() < 7 || pos->getY() > 9) {
			return false;
		}
	}
	return true;
}

vector<shared_ptr<Position>> Advisor::getAvaliablePlace(int width, int height)
{
	vector<shared_ptr<Position>> avaliablePlace;
	for (auto step : steps) {
		shared_ptr<Position> dest(new Position(pos->getX() + step[0], pos->getY() + step[1]));
		if (!onBoard(dest)) {
			continue;
		}
		avaliablePlace.push_back(dest);
	}
	return avaliablePlace;
}

bool Elephant::onBoard(shared_ptr<Position> pos)
{
	if (playerIndex == 1) {
		if (pos->getX() < 0 || pos->getX() > 8 || pos->getY() < 0 || pos->getY() > 4) {
			return false;
		}
	}
	else {
		if (pos->getX() < 0 || pos->getX() > 8 || pos->getY() < 4 || pos->getY() > 9) {
			return false;
		}
	}
	return true;
}

vector<shared_ptr<Position>> Elephant::getAvaliablePlace(int width, int height)
{
	vector<shared_ptr<Position>> avaliablePlace;
	for (auto step : steps) {
		shared_ptr<Position> dest(new Position(pos->getX() + step[0], pos->getY() + step[1]));
		if (!onBoard(dest)) {
			continue;
		}
		avaliablePlace.push_back(dest);
	}

	return avaliablePlace;
}

vector<shared_ptr<Position>> Horse::getAvaliablePlace(int width, int height)
{
	vector<shared_ptr<Position>> avaliablePlace;
	for (auto step : steps) {
		shared_ptr<Position> dest(new Position(pos->getX() + step[0], pos->getY() + step[1]));
		if (!onBoard(dest)) {
			continue;
		}
		avaliablePlace.push_back(dest);
	}

	return avaliablePlace;
}

vector<shared_ptr<Position>> Soldier::getAvaliablePlace(int width, int height)
{
	vector<shared_ptr<Position>> avaliablePlace;
	if (isCrossRiver) {
		for (auto step : steps) {
			shared_ptr<Position> dest(new Position(0, 0));
			if (playerIndex == 1) {
				dest->setX(pos->getX() + step[0]);
				dest->setY(pos->getY() + step[1]);
			}
			else {
				dest->setX(pos->getX() - step[0]);
				dest->setY(pos->getY() - step[1]);
			}
			
			if (!onBoard(dest)) {
				continue;
			}
			avaliablePlace.push_back(dest);
		}
	}
	else {
		shared_ptr<Position> dest(new Position(0, 0));
		if (playerIndex == 1) {
			dest->setX(pos->getX() + steps[0][0]);
			dest->setY(pos->getY() + steps[0][1]);
		}
		else {
			dest->setX(pos->getX() - steps[0][0]);
			dest->setY(pos->getY() - steps[0][1]);
		}
		
		if (onBoard(dest)) {
			avaliablePlace.push_back(dest);
		}
	}
	return avaliablePlace;
}

//
//void Piece::load(ifstream& inFile)
//{
//	string wholeLineString;
//	stringstream wholeLineStream;
//	bool continueReading = true;
//	int colonLocation;
//
//	// go through file
//	while (!inFile.eof() && continueReading) {
//		// read the whole line
//		getline(inFile, wholeLineString);
//
//		// find the colon and prepare to read from stringstream after the colon
//		if ((colonLocation = wholeLineString.find(":")) != string::npos)
//			wholeLineStream.str(wholeLineString.substr(colonLocation + 1));
//
//		// when the find() function doesn't find it, string::npos is returned
//		if (wholeLineString.find("pieceID") != string::npos) {
//			pieceID = StringPlus::trim(
//				wholeLineString.substr(colonLocation + 1));
//		}
//		else if (wholeLineString.find("x") != string::npos) {
//			wholeLineStream >> loc.x;
//		}
//		else if (wholeLineString.find("y") != string::npos) {
//			wholeLineStream >> loc.y;
//		}
//		else if (wholeLineString.find("Piece End") != string::npos) {
//			continueReading = false;
//		}
//		wholeLineStream.clear(); // get ready for next line
//	}
//
//}