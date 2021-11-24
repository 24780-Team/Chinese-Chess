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
	vector<Position*> placesOfPieces;
	currBoard->getPlacesOfPieces(playerIndex, placesOfPieces);
	int index = rand() % placesOfPieces.size();
	Position* origin = placesOfPieces[index];
	bool canMove = false;
	Piece* piece;

	vector<Position*> avaliablePlaces;
	while (!canMove) {
		piece = currBoard->getPiece(origin);
		currBoard->getAvaliblePlaces(piece, avaliablePlaces);
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
	if (currBoard->lossGeneral(currPlayerIndex)) {
		if (currPlayerIndex == playerIndex) {
			currNode->move->score = INT_MIN;
		}
		else {
			currNode->move->score = calcScore();
		}
		return;
	}
	if (currLevel == 0) {
		currNode->move->score = calcScore();
		return;
	}

	vector<Position*> placesOfPieces;
	currBoard->getPlacesOfPieces(currPlayerIndex, placesOfPieces);
	for (auto origin : placesOfPieces) {
		Piece* piece = currBoard->getPiece(origin);
		vector<Position*> avaliablePlaces;
		currBoard->getAvaliblePlaces(piece, avaliablePlaces);

		for (auto dest : avaliablePlaces) {
			Piece* eliminatedPiece = currBoard->setPiece(dest, piece);
			Move* move = new Move(origin, dest, 0);
			TreeNode* leafNode = new TreeNode(move);
			currNode->leaves.push_back(leafNode);
			dfs(currLevel - 1, leafNode, 1 - currPlayerIndex);
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
	srand(time(NULL));
	if (searchLevel == 0) return getRandomMove();
	counter = 0;
	Move* move = new Move(nullptr, nullptr, 0);
	TreeNode* root = new TreeNode(move);
	dfs(searchLevel, root, playerIndex);
	cout << counter << " times dfs" << endl;
	int maxScore = move->score;
	vector<Move*> choices;
	for (auto leaf : root->leaves) {
		if (leaf->move->score == maxScore) {
			choices.push_back(leaf->move);
		}
	}
	int index = rand() % choices.size();
	move = new Move(choices[index]);
	delete root;
	return move;
}

int AIPlayer::calcScore()
{
	int score = currBoard->calcScore(1 - playerIndex) - currBoard->calcScore(playerIndex);
	return score;
}
