
#include "Board.h"


Board::Board(int rows, int columns, char* backgroundFile, int numGemTypes, Point gemSize)
{

	mBackgroundFileName = std::string(backgroundFile);
	

	mRows = rows;
	mColumns = columns;

	mTotalElements = rows*columns;

	mNumGemTypes = numGemTypes;

	mTiles = std::vector<Gem*>(mTotalElements);
	
	mDrawingScreen = NULL;

	mInitialized = false;
	mGemSize = gemSize;
	//mGemSize.setX(GemW);
	//mGemSize.setY(GemH);
	
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
	mBackground = IMG_Load(mBackgroundFileName.c_str());
	if(mBackground == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", mBackgroundFileName, SDL_GetError());
		return;
	}

	srand (time(NULL));

	vector<string> gemsAssets;
	gemsAssets.push_back("assets/art/Gems/green.png");
	gemsAssets.push_back("assets/art/Gems/blue.png");
	gemsAssets.push_back("assets/art/Gems/red.png");
	gemsAssets.push_back("assets/art/Gems/yellow.png");
	gemsAssets.push_back("assets/art/Gems/purple.png");

	
	for(int i = 0; i < mTotalElements; i++)
	{
		int type = rand() % mNumGemTypes;
		mTiles[i] = new Gem(Point(i%mColumns, i/mRows), type, (char*)gemsAssets.at(type).c_str());
		mTiles[i]->setScreen(mDrawingScreen);
		mTiles[i]->init();
	}

	mInitialized = true;
}

void Board::mouseOver(int x, int y)
{
	if(x > mBackground->w || y > mBackground->h || x < 0 || y < 0)
		return;

	
	int indexX = ((int)x / mGemSize.getX());
	int indexY =  ((int)y / mGemSize.getY());

	if(indexX + indexY*mColumns > mTotalElements || indexX > mColumns || indexY > mRows)
		return;
	mTiles[indexX + indexY*mColumns]->mouseOver(true); 
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
		mTiles[i]->render( (i%mColumns) * mGemSize.getX(), ((int)(i/mRows))*mGemSize.getY()); 
	}

	SDL_UpdateRects(mDrawingScreen, 1, &dest);
}

void Board::setScreen(SDL_Surface *screen)
{
	mDrawingScreen = screen;
}