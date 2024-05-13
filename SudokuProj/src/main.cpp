/* Play Sudoku */
#include "Sudoku.h"

int main(int argc, char* argv[])
{
	// Create Sudoku game object
	Sudoku::Sudoku S;

	// Play Sudoku game
	S.play();
	if (S.life ==0)
	{
		S.~Sudoku();
	}
	
	S.Youlost();
	return 0;
}