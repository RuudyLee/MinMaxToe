#pragma once

enum class TTTVal { NIL = 0, TIE, X, O };

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

	// temporary
	void PrintBoard();

private:
	bool FullBoard();

private:
	TTTVal _Board[3][3];
};

