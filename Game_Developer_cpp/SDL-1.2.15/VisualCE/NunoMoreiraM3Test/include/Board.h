
#ifndef BOARD_H
#define BOARD_H

#include "Gem.h"
#include <vector>
#include "SDL.h"


class Board
{

public:
	Board(int r, int c, char* backgroundFile);
	~Board();
	void update(float dt);
	void render(SDL_Surface *screen, int x, int y);

private:
	int mRows;
	int mColumns;
	int mTotalElements;
	SDL_Surface *background;
	std::vector<Gem*> mTiles;

};

#endif