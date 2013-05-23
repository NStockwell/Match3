
#include "Board.h"


Board::Board(int rows, int columns, char* backgroundFile)
{

	mBackgroundFileName = std::string(backgroundFile);
	

	mRows = rows;
	mColumns = columns;

	mTotalElements = rows*columns;

	mTiles = std::vector<Gem*>(mTotalElements);
	
	mDrawingScreen = NULL;

	mInitialized = false;
	
	
}

Board::~Board()
{
	for(int i = mTiles.size() - 1; i >= 0; i--)
	{
		delete mTiles[i];
	}
}

void Board::init()
{
	//needs to be a bmp
	mBackground = IMG_Load(mBackgroundFileName.c_str());
	if(mBackground == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", mBackgroundFileName, SDL_GetError());
		return;
	}

	srand (time(NULL));

	for(int i = 0; i < mTotalElements; i++)
	{
		mTiles[i] = new Gem(rand() % 5, "assets/art/Gems/green.png");
		mTiles[i]->setScreen(mDrawingScreen);
		mTiles[i]->init();
	}

	mInitialized = true;
}

void Board::update(float dt)
{
	if(!mInitialized)
		return;
}

void Board::render(int x, int y)
{
	if(!mInitialized)
		return;

	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = mBackground->w;
	dest.h = mBackground->h;

	SDL_BlitSurface(mBackground, NULL, mDrawingScreen, &dest);

	for(int i = 0; i < mTotalElements; i++)
	{
		mTiles[i]->render( (i%mColumns) * 35, ((int)(i/mRows))*35); 
	}

	SDL_UpdateRects(mDrawingScreen, 1, &dest);
}

void Board::setScreen(SDL_Surface *screen)
{
	mDrawingScreen = screen;
}