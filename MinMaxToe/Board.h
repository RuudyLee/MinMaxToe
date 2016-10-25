/* Create by: Rudy Lee */

#pragma once

// Values for board state
enum class TTTVal { NIL = 0, TIE, X, O };

/*
* Encapsulates the board states into a 3x3 matrix (a tic-tac-toe board)
*/
class Board
{
public:
	Board();
	~Board();

	void Init();
	TTTVal GetValue(int x, int y);
	void SetValue(int x, int y, TTTVal val);
	TTTVal CheckWinner();
	bool GameOver();

private:
	bool FullBoard();

private:
	TTTVal _Board[3][3];
};

