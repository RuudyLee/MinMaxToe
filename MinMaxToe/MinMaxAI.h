#pragma once
#include "Board.h"
#include <vector>

struct MoveNode {
	int x;
	int y;
	int score;

	MoveNode() {}
	MoveNode(int sc) {
		score = sc;
	}

	void setMove(int _x, int _y) {
		x = _x;
		y = _y;
	}
};

class MinMaxAI
{
public:
	MinMaxAI();
	MinMaxAI(TTTVal aiPlayer);
	~MinMaxAI();
	
	void init(TTTVal aiPlayer);
	void performMove(Board *board);
	
	int evaluations = 0;
private:
	MoveNode getBestMove(Board board, int min, int max, TTTVal player);

	TTTVal _humanPlayer;
	TTTVal _aiPlayer;
};

