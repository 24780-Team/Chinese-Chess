#pragma once
#include <string>

class Player {
private:
    std::string name;
    int index;

public:
    std::string getName() { return name; }
    int getIndex() { return index; }
    void setName(); // Need to be changed.
    void setIndex(int index) { this->index = index; }
};