#include <iostream>

#include "Game.h"

using namespace std;

int main() {
    Game *game = new Game();
    game->startGame();
    bool terminate = false;
    while (!terminate) {
        game->draw();
        terminate = game->nextTurn();
    }
}