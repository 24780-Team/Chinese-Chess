#pragma once
#include <vector>

#include "Piece.h"

class Board {
    private:
        std::vector<std::vector<Piece*>> posToPiece;
        int width = 9;
        int height = 10;
        bool general0Alive;
        bool general1Alive;

        void initializeGeneral();
        void initializeAdvisor();
        void initializeElephant();
        void initializeHorse();
        void initializeChariot();
        void initializeCannon();
        void initializeSoldier();
    
    public:
        Board();
        Piece* getPiece(Position* pos);
        void setPiece(Position* pos, Piece* piece);
        void addPiece(Piece *piece); // To be implemented.
        std::vector<Position*> getAvaliblePlaces(Piece *piece);
        int getWinner();

        void draw(); // To be implemented.
};