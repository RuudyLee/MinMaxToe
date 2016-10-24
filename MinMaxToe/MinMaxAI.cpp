#include "MinMaxAI.h"
#include <vector>
#include <algorithm>
#include <iostream>

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
	evaluations = 0;
	MoveNode bestMove = getBestMove(*board, -99999, 99999, _aiPlayer);
	board->SetValue(bestMove.x, bestMove.y, _aiPlayer);
	
}

/*
* My Implementation of the MinMax Algorithm with alpha-beta pruning
*/ 
MoveNode MinMaxAI::getBestMove(Board board, int alpha, int beta, TTTVal player) {
	evaluations++;
	// Terminal nodes
	TTTVal winner = board.CheckWinner();
	if (winner == _aiPlayer) {
		return MoveNode(1);
	}
	else if (winner == _humanPlayer) {
		return MoveNode(-1);
	}
	else if (winner == TTTVal::TIE) { 
		return MoveNode(0);
	}

	std::vector<MoveNode> children;

	// MinMax Recursion
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			if (board.GetValue(x, y) == TTTVal::NIL) {
				MoveNode move;
				move.setMove(x, y);
				board.SetValue(x, y, player);
				
				if (player == _aiPlayer) { // Maximizing player
					move.score = getBestMove(board, alpha, beta, _humanPlayer).score;
					alpha = std::max(alpha, move.score);
					// check for beta cut-off
					if (beta <= alpha) {
						return MoveNode(beta);
					}
				}
				else { // Minimizing player
					move.score = getBestMove(board, alpha, beta, _aiPlayer).score;
					beta = std::min(beta, move.score);
					// check for alpha cut-off
					if (beta <= alpha) {
						return MoveNode(alpha);
					}
				}
				children.push_back(move);
				board.SetValue(x, y, TTTVal::NIL);
			}
		}
	}

	// Determine best move
	MoveNode bestMove;
	if (player == _aiPlayer) {
		// wants highest score
		bestMove.score = -99999;
		for (MoveNode move : children) {
			if (move.score > bestMove.score) {
				bestMove = move;
			}
		}
	}
	else
	{
		// wants lowest score
		bestMove.score = 99999;
		for (MoveNode move : children) {
			if (move.score < bestMove.score) {
				bestMove = move;
			}
		}
	}

	return bestMove;
}

