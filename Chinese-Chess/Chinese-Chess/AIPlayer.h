#pragma once
#include <vector>
#include "Board.h"

struct Move {
	shared_ptr<Position> origin = nullptr;
	shared_ptr<Position> dest = nullptr;
	int score = 0;
	Move(shared_ptr<Position> origin, shared_ptr<Position> dest, int score) {
		this->origin = origin;
		this->dest = dest;
		this->score = score;
	}
	Move(const Move* move) {
		origin = move->origin;
		dest = move->dest;
		score = move->score;
	}
};

struct TreeNode {
	Move* move = nullptr;
	vector<TreeNode*> children;
	TreeNode(Move* move) {
		this->move = move;
	}
	~TreeNode() { 
		delete move;
		if (this->children.size() != 0) {
			for (auto child : children) {
				delete child;
			}
			//children.clear();
			vector<TreeNode*>().swap(children);
		}
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

