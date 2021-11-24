#pragma once
#include <vector>
#include "Board.h"

struct Move {
	Position* origin = nullptr;
	Position* dest = nullptr;
	int score = 0;
	Move(Position* origin, Position* dest, int score) {
		this->origin = origin;
		this->dest = dest;
		this->score = score;
	}
};

struct TreeNode {
	Move* move = nullptr;
	vector<TreeNode*> leaves;
	TreeNode(Move* move) {
		this->move = move;
	}
};

class AIPlayer {
private:
	Board* currBoard;
	int playerIndex;
	int searchLevel;
	int calcScore();
	Move* getRandomMove();
	void dfs(int currLevel, TreeNode* currNode, int currPlayerIndex);

public:
	AIPlayer(Board* board, int playerIndex, int searchLevel);
	Move* getNextMove();
};

