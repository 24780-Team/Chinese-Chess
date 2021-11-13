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

        void initializeGeneral(int &index);
        void initializeAdvisor(int& index);
        void initializeElephant(int& index);
        void initializeHorse(int& index);
        void initializeChariot(int& index);
        void initializeCannon(int& index);
        void initializeSoldier(int& index);

        std::vector<Position*> checkForElephant(Piece* piece, std::vector<Position*>& positions);
        std::vector<Position*> checkForHorse(Piece* piece, std::vector<Position*>& positions);
        std::vector<Position*> checkForGeneral(Piece* piece, std::vector<Position*>& positions);
        std::vector<Position*> checkForChariot(Piece* piece);
        std::vector<Position*> checkForCannon(Piece* piece);

    
    public:
        Board();
        Piece* getPiece(Position* pos);
        void setPiece(Position* pos, Piece* piece);
        void addPiece(Piece *piece); // Need to be implemented.
        std::vector<Position*> getAvaliblePlaces(Piece *piece);
        int getWinner();

        void draw(); // // Need to be changed.
};