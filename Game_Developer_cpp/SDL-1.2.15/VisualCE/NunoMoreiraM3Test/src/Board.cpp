
#include "Board.h"


Board::Board(int rows, int columns, char* backgroundFile)
{

	
	//needs to be a bmp
	background = SDL_LoadBMP(backgroundFile);
	if(background == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", backgroundFile, SDL_GetError());
		return;
	}

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

void Board::update(float dt)
{

}

void Board::render(SDL_Surface *screen, int x, int y)
{
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = background->w;
	dest.h = background->h;

	SDL_BlitSurface(background, NULL, screen, &dest);

	SDL_UpdateRects(screen, 1, &dest);
}