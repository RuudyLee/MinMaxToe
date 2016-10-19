#include "Board.h"
#include <iostream>


Board::Board()
{
}


Board::~Board()
{
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
		if (_Board[0][i] == _Board[1][i] && _Board[0][i] == _Board[2][i]) {
			return _Board[0][i];
		}
		if (_Board[i][0] == _Board[i][1] && _Board[i][0] == _Board[i][2]) {
			return _Board[i][0];
		}
	}

	// check diagonals
	if (_Board[0][0] == _Board[1][1] && _Board[0][0] == _Board[2][2]) {
		return _Board[0][0];
	}
	if (_Board[2][0] == _Board[1][1] && _Board[2][0] == _Board[0][2]) {
		return _Board[2][0];
	}

	return TTTVal::NIL;
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

void Board::PrintBoard() {
	system("cls");
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			std::cout << " ";
			switch (_Board[x][y]) {
			case TTTVal::X:
				std::cout << "X";
				break;
			case TTTVal::O:
				std::cout << "O";
				break;
			case TTTVal::NIL:
				std::cout << " ";
				break;
			}
			std::cout << std::endl;
		}
	}
}