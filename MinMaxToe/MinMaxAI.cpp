#include "MinMaxAI.h"
#include <vector>


MinMaxAI::MinMaxAI() {
}

MinMaxAI::MinMaxAI(TTTVal aiPlayer) {
	init(aiPlayer);
}

MinMaxAI::~MinMaxAI() {
}

/*
* Initializes AI
*/
void MinMaxAI::init(TTTVal aiPlayer) {
	_aiPlayer = aiPlayer;
	if (_aiPlayer == TTTVal::X) {
		_humanPlayer = TTTVal::O;
	}
	else {
		_humanPlayer = TTTVal::X;
	}
}

/*
* Plays the best move to the given board using the MinMax algorithm
*/
void MinMaxAI::performMove(Board *board) {
	AIMove bestMove = getBestMove(*board, _aiPlayer);
	board->SetValue(bestMove.x, bestMove.y, _aiPlayer);
}

/*
* My Implementation of the MinMax Algorithm
*/
AIMove MinMaxAI::getBestMove(Board board, TTTVal player) {
	// Base case
	TTTVal winner = board.CheckWinner();
	if (winner == _aiPlayer) {
		return AIMove(1);
	}
	else if (winner == _humanPlayer) {
		return AIMove(-1);
	}
	else if (winner == TTTVal::TIE) { 
		return AIMove(0);
	}

	std::vector<AIMove> moves;

	// MinMax Recursion
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			if (board.GetValue(x, y) == TTTVal::NIL) {
				AIMove move;
				move.x = x;
				move.y = y;
				board.SetValue(x, y, player);
				if (player == _aiPlayer) {
					move.score = getBestMove(board, _humanPlayer).score;
				}
				else {
					move.score = getBestMove(board, _aiPlayer).score;
				}
				moves.push_back(move);
				board.SetValue(x, y, TTTVal::NIL);
			}
		}
	}

	// Determine best move
	AIMove bestMove;
	if (player == _aiPlayer) {
		// wants +10
		int bestScore = -99999;
		for (AIMove move : moves) {
			if (move.score > bestScore) {
				bestMove = move;
				bestScore = move.score;
			}
		}
	}
	else
	{
		// wants -10
		int bestScore = 99999;
		for (AIMove move : moves) {
			if (move.score < bestScore) {
				bestMove = move;
				bestScore = move.score;
			}
		}
	}

	return bestMove;
}

