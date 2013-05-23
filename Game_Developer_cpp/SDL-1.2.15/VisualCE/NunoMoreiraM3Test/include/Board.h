
#ifndef BOARD_H
#define BOARD_H

#include "Gem.h"
#include <vector>

class Board
{

public:
	Board(int r, int c);
	~Board();

private:
	int mRows;
	int mColumns;
	int mTotalElements;
	std::vector<Gem*> mTiles;

};

#endif