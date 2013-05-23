
#include "Board.h"


Board::Board(int rows, int columns)
{
	mRows = rows;
	mColumns = columns;

	mTotalElements = rows*columns;

	mTiles = std::vector<Gem*>(mTotalElements);
}

Board::~Board()
{
	for(int i = mTiles.size() - 1; i >= 0; i--)
	{
		delete mTiles[i];
	}
}