#pragma once

using namespace std;

class Position {
private:
	int x;
	int y;

public:
	Position(int x, int y) {
		setX(x);
		setY(y);
	}
	int getX() { return x; };
	int getY() { return y; };
	void setX(int x) { this->x = x; };
	void setY(int y) { this->y = y; };
	bool operator== (const Position otherPos) {
		return (x == otherPos.x && (y == otherPos.y));
	}
};