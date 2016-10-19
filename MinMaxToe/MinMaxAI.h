#pragma once
#include "Board.h"

struct AIMove {
	int x;
	int y;
	int score;

	AIMove() {}
	AIMove(int sc) {
		score = sc;
	}
};

class MinMaxAI
{
public:
	MinMaxAI();
	~MinMaxAI();
	
	void init(TTTVal aiPlayer);
	void performMove(Board board);

private:
	AIMove getBestMove(Board board, TTTVal player);

	TTTVal _humanPlayer;
	TTTVal _aiPlayer;
};

