#include <iostream>
#include <time.h>

#include "AIPlayer.h"

using namespace std;

int counter = 0;

AIPlayer::AIPlayer(Board* board, int playerIndex, int searchLevel)
{
	currBoard = board;
	this->playerIndex = playerIndex;
	this->searchLevel = searchLevel;
}

Move* AIPlayer::getRandomMove() {
	srand(time(NULL));
	vector<Position*> placesOfPieces = currBoard->getPlacesOfPieces(playerIndex);
	int index = rand() % placesOfPieces.size();
	Position* origin = placesOfPieces[index];
	bool canMove = false;
	Piece* piece;
	vector<Position*> avaliablePlaces;
	while (!canMove) {
		piece = currBoard->getPiece(origin);
		avaliablePlaces = currBoard->getAvaliblePlaces(piece);
		if (avaliablePlaces.size() > 0) {
			canMove = true;
		}
	}
	index = rand() % avaliablePlaces.size();
	Position* dest = avaliablePlaces[index];
	return new Move(origin, dest, 0);
}

void AIPlayer::dfs(int currLevel, TreeNode* currNode, int currPlayerIndex) {
	counter += 1;
	if (currLevel == 0 || currBoard->lossGeneral(currPlayerIndex)) {
		currNode->move->score = calcScore();
		return;
	}

	int nextLevel;
	if (currPlayerIndex == playerIndex) {
		nextLevel = currLevel;
	}
	else {
		nextLevel = currLevel - 1;
	}

	vector<Position*> placesOfPieces = currBoard->getPlacesOfPieces(currPlayerIndex);
	for (auto origin : placesOfPieces) {
		Piece* piece = currBoard->getPiece(origin);
		vector<Position*> avaliablePlaces = currBoard->getAvaliblePlaces(piece);

		for (auto dest : avaliablePlaces) {
			Piece* eliminatedPiece = currBoard->setPiece(dest, piece);
			Move* move = new Move(origin, dest, 0);
			TreeNode* leafNode = new TreeNode(move);
			currNode->leaves.push_back(leafNode);
			dfs(nextLevel, leafNode, 1 - currPlayerIndex);
			currBoard->setPiece(origin, piece);
			if (eliminatedPiece != nullptr) {
				currBoard->setAlive(eliminatedPiece);
			}
		}
	}

	if (currPlayerIndex == playerIndex) {
		int maxScore = INT_MIN;
		for (auto leaf : currNode->leaves) {
			maxScore = max(maxScore, leaf->move->score);
		}
		currNode->move->score = maxScore;
	}
	else {
		int minScore = INT_MAX;
		for (auto leaf : currNode->leaves) {
			minScore = min(minScore, leaf->move->score);
		}
		currNode->move->score = minScore;
	}

}

Move* AIPlayer::getNextMove()
{
	if (searchLevel == 0) return getRandomMove();
	counter = 0;
	Move* move = new Move(nullptr, nullptr, 0);
	TreeNode* root = new TreeNode(move);
	dfs(searchLevel, root, playerIndex);
	cout << counter << " times dfs" << endl;
	int maxScore = INT_MIN;
	for (auto leaf : root->leaves) {
		if (leaf->move->score > maxScore) {
			maxScore = leaf->move->score;
			move = leaf->move;
		}
	}
	return move;
}

int AIPlayer::calcScore()
{
	int score = currBoard->calcScore(1 - playerIndex) - currBoard->calcScore(playerIndex);
	return score;
}
