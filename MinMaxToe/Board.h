#pragma once

enum class TTTVal { NIL = 0, TIE, X, O };

class Board
{
public:
	Board();
	~Board();

	TTTVal GetValue(int x, int y);
	void SetValue(int x, int y, TTTVal val);
	TTTVal CheckWinner();
	bool FullBoard();

	// temporary
	void PrintBoard();

private:
	TTTVal _Board[3][3];
};

