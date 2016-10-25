/* Create by: Rudy Lee */

#include "Board.h"
#include <iostream>


Board::Board() {
}


Board::~Board() {
}

/*
* Initializes all the board values to TTTVal::NIL
*/
void Board::Init() {
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			_Board[x][y] = TTTVal::NIL;
		}
	}
}

/*
* Gets the value at (x,y)
*/
TTTVal Board::GetValue(int x, int y) {
	return _Board[x][y];
}

/*
* Sets the value to given 'val' at (x,y)
*/
void Board::SetValue(int x, int y, TTTVal val) {
		_Board[x][y] = val;
}

/*
* Checks board state to see if anyone has won yet
* Returns TTTVal of the winner and TTTVal::NIL if there is no winner
*/
TTTVal Board::CheckWinner() {
	// check rows & columns
	for (int i = 0; i < 3; i++) {
		// check rows
		if (_Board[0][i] != TTTVal::NIL && 
			_Board[0][i] == _Board[1][i] && 
			_Board[0][i] == _Board[2][i]) {
			return _Board[0][i];
		}

		// check columns
		if (_Board[i][0] != TTTVal::NIL &&
			_Board[i][0] == _Board[i][1] && 
			_Board[i][0] == _Board[i][2]) {
			return _Board[i][0];
		}
	}

	// check diagonals
	if (_Board[0][0] != TTTVal::NIL &&
		_Board[0][0] == _Board[1][1] && 
		_Board[0][0] == _Board[2][2]) {
		return _Board[0][0];
	}
	if (_Board[2][0] != TTTVal::NIL &&
		_Board[2][0] == _Board[1][1] && 
		_Board[2][0] == _Board[0][2]) {
		return _Board[2][0];
	}

	// Return TTTVal::TIE if the board is full
	if (FullBoard()) {
		return TTTVal::TIE;
	}

	// Otherwise, there has been no winner
	return TTTVal::NIL;
}

/*
*  Returns if the board state's end condition has been reached
*/
bool Board::GameOver() {
	return CheckWinner() != TTTVal::NIL;
}

/*
* Checks if the board is full
*/
bool Board::FullBoard() {
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			if (_Board[x][y] == TTTVal::NIL) {
				return false;
			}
		}
	}
	return true;
}