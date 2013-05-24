
#ifndef BOARD_H
#define BOARD_H

#include "Gem.h"
#include "Point.h"
#include <vector>
#include "SDL.h"
#include "SDL_image.h"

#include <stdlib.h>
#include <time.h>

using namespace std;
class Board
{

public:
	Board(int r, int c, char* backgroundFile, int numGemTypes, Point gemSize);
	~Board();
	void setScreen(SDL_Surface *screen);
	void update(float dt);
	void render();
	void render(int x, int y);
	void init();
	void mouseOver(int x, int y);
	void setPosition(int x, int y);

private:
	int mRows;
	int mColumns;
	int mTotalElements;
	int mNumGemTypes;
	bool mInitialized;
	Point mPosition;
	Point mSize;
	Point mGemSize;
	std::string mBackgroundFileName;
	SDL_Surface *mBackground;
	SDL_Surface *mDrawingScreen;
	std::vector<Gem*> mTiles;

};

#endif