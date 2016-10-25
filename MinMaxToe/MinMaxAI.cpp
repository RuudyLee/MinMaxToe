/* Create by: Rudy Lee */

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
* Initializes AI to a given TTTVal
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
* Plays the best move to the given board using the MinMax 
* or Alpha-Beta algorithm, depending on the boolean 'enableAlphaBeta'
*/
void MinMaxAI::performMove(Board *board) {
	evaluations = 0;
	MoveNode bestMove = getBestMove(*board, -99999, 99999, _aiPlayer);
	std::cout << evaluations << std::endl; // for testing
	board->SetValue(bestMove.x, bestMove.y, _aiPlayer);
	
}

/*
* Toggles the switch for Alpha-Beta Pruning
*/
void MinMaxAI::toggleAlphaBeta() {
	enableAlphaBeta = !enableAlphaBeta;
}

/*
* My Implementation of the MinMax & Alpha-Beta Pruning Search Algorithms
*/ 
MoveNode MinMaxAI::getBestMove(Board board, int alpha, int beta, TTTVal player) {
	// Count the number of evaluations (for testing)
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

	// Recursively goes through each node and its children
	std::vector<MoveNode> children;

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			if (board.GetValue(x, y) == TTTVal::NIL) { // validate move
				MoveNode move;
				move.setMove(x, y);
				board.SetValue(x, y, player);
				
				if (player == _aiPlayer) { // Maximizing player
					move.score = getBestMove(board, alpha, beta, _humanPlayer).score;
					alpha = std::max(alpha, move.score);
					// check for beta cut-off
					if (enableAlphaBeta && beta <= alpha) {
						return MoveNode(beta);
					}
				}
				else { // Minimizing player
					move.score = getBestMove(board, alpha, beta, _aiPlayer).score;
					beta = std::min(beta, move.score);
					// check for alpha cut-off
					if (enableAlphaBeta && beta <= alpha) {
						return MoveNode(alpha);
					}
				}
				children.push_back(move);

				// evaluation of this branch complete, return board state to normal
				board.SetValue(x, y, TTTVal::NIL);
			}
		}
	}

	// Determine the move with the best score (max/min)
	MoveNode bestMove;
	if (player == _aiPlayer) { // Maximizing player
		bestMove.score = -99999;
		for (MoveNode move : children) {
			if (move.score > bestMove.score) {
				bestMove = move;
			}
		}
	}
	else { // Minimizing Player
		bestMove.score = 99999;
		for (MoveNode move : children) {
			if (move.score < bestMove.score) {
				bestMove = move;
			}
		}
	}

	return bestMove;
}

