/* Create by: Rudy Lee */

#pragma once
#include "Board.h"
#include <vector>

/*
* Encapsulates nodes for the MinMax Tree
*/
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

/*
* An AI program for Tic-Tac-Toe
* Uses the Alpha-Beta Pruning search algorithm
*/
class MinMaxAI
{
public:
	MinMaxAI();
	MinMaxAI(TTTVal aiPlayer);
	~MinMaxAI();
	
	void init(TTTVal aiPlayer);
	void performMove(Board *board);
	void toggleAlphaBeta();

	int evaluations = 0;
private:
	MoveNode getBestMove(Board board, int min, int max, TTTVal player);
	bool enableAlphaBeta = true;

	TTTVal _humanPlayer;
	TTTVal _aiPlayer;
};

