#pragma once

class Position {
private:
    int x;
    int y;

public:
    Position(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int getX() { return x; }
    int getY() { return y; }
    bool operator== (const Position otherPos) {
        return (x == otherPos.x) && (y == otherPos.y);
    }
};