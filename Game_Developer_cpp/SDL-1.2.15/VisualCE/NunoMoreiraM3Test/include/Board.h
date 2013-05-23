
#ifndef BOARD_H
#define BOARD_H

#include "Gem.h"
#include <vector>
#include "SDL.h"
#include "SDL_image.h"

#include <stdlib.h>
#include <time.h>


class Board
{

public:
	Board(int r, int c, char* backgroundFile);
	~Board();
	void setScreen(SDL_Surface *screen);
	void update(float dt);
	void render(int x, int y);
	void init();

private:
	int mRows;
	int mColumns;
	int mTotalElements;
	bool mInitialized;
	std::string mBackgroundFileName;
	SDL_Surface *mBackground;
	SDL_Surface *mDrawingScreen;
	std::vector<Gem*> mTiles;

};

#endif