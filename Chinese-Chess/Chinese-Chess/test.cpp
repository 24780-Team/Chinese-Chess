#include <iostream>

#include "Game.h"

using namespace std;

int main() {
    const int width = 1400, height = 1200;
    int key;
    FsOpenWindow(16, 16, width, height, 1, "Chinese-Chess (Twisted Fate)"); // use double buffer

    Game *game = new Game();
    bool quit = game->startGame();
    if (quit) {
        return 0;
    }
    game->playBGM();

    bool terminate = false;
    while (!terminate) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        FsPollDevice();
        key = FsInkey();

        switch (key) {
        case FSKEY_ESC:
            break;
        }
        terminate = game->nextTurn();
        game->draw();
        FsSwapBuffers();
        FsSleep(20);
    }

    return 0;
}