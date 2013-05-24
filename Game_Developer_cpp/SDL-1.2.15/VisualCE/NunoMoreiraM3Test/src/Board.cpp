
#include "Board.h"


Board::Board(int rows, int columns, /*char* backgroundFile, */int numGemTypes, Point gemSize)
{

	/*mBackgroundFileName = std::string(backgroundFile);*/
	

	mRows = rows;
	mColumns = columns;

	mTotalElements = rows*columns;

	mNumGemTypes = numGemTypes;

	mTiles = std::vector<Gem*>(mTotalElements);
	
	mDrawingScreen = NULL;

	mInitialized = false;
	mSwitching = false;
	mGemSize = gemSize;

	mSize.setX(mColumns * mGemSize.getX());
	mSize.setY(mRows * mGemSize.getY());

	mSelectedGemIndex.setX(-1);// = NULL;
	mSelectedGemIndex.setY(-1);
	
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
	/*mBackground = IMG_Load(mBackgroundFileName.c_str());
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
	gemsAssets.push_back("assets/art/Gems/purple.png");*/

	
	for(int i = 0; i < mTotalElements; i++)
	{
		
		//int type = rand() % mNumGemTypes;
		mTiles[i] = GemManager::getInstance().getRandomTypeGem();//new Gem(type, (char*)gemsAssets.at(type).c_str());
		mTiles[i]->setPosition(mPosition.getX() + (i%mColumns) * mGemSize.getX(), mPosition.getY() + (i/mRows) * mGemSize.getY() );
		mTiles[i]->setScreen(mDrawingScreen);
		mTiles[i]->init();
	}

	mInitialized = true;
}

bool Board::insideBoundaries(int x, int y)
{
	if(x >= mSize.getX() + mPosition.getX()  || y >= mSize.getY() + mPosition.getY() || x < mPosition.getX() || y < mPosition.getY())
		return false;
	return true;
}

void Board::setPosition(int x, int y)
{
	mPosition.setX(x);
	mPosition.setY(y);
}

Point Board::coordinateToIndex(int x, int y)
{
	int indexX = (int)((x - mPosition.getX()) / mGemSize.getX());
	int indexY =  (int)((y - mPosition.getY()) / mGemSize.getY());
	return Point(indexX, indexY);
}

int Board::XYCoordinatesToIndex(Point p)
{
	return XYCoordinatesToIndex(p.getX(), p.getY());
}

int Board::XYCoordinatesToIndex(int x, int y)
{
	return x + y*mColumns;
}

void Board::mousePressed(int x, int y)
{
	if(!insideBoundaries(x,y) || mSwitching)
		return;
	
	Point xy = coordinateToIndex(x,y);
	int index = XYCoordinatesToIndex(xy);
	
	if(mSelectedGemIndex.getX() != -1)
	{
		switchGems(mSelectedGemIndex, xy);
		mSelectedGemIndex.setX(-1);// = NULL;
	}
	else
	{
		mSelectedGemIndex = xy;// = mTiles[index];
	}
	mTiles[index]->mousePressed(true); 
}

void Board::switchGems(Point g1, Point g2)
{
	mSwitching = true;
	Gem* tempGem = mTiles[XYCoordinatesToIndex(g1)];
	mTiles[XYCoordinatesToIndex(g1)] =  mTiles[XYCoordinatesToIndex(g2)];
	mTiles[XYCoordinatesToIndex(g2)] = tempGem;
	mSwitching = false;
}


void Board::mouseOver(int x, int y)
{
	if(!insideBoundaries(x,y) || mSwitching)
		return;
	
	Point index = coordinateToIndex(x,y);
	
	mTiles[index.getX() + index.getY()*mColumns]->mouseOver(true); 
}

void Board::update(float dt)
{
	if(!mInitialized)
		return;
}

void Board::render()
{
	render(mPosition.getX(), mPosition.getY());
}

void Board::render(int x, int y)
{
	if(!mInitialized)
		return;

	/*SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = mBackground->w;
	dest.h = mBackground->h;*/

	//SDL_BlitSurface(mBackground, NULL, mDrawingScreen, &dest);

	for(int i = 0; i < mTotalElements; i++)
	{
		mTiles[i]->render( );//(i%mColumns) * mGemSize.getX(), ((int)(i/mRows))*mGemSize.getY()); 
	}

	//SDL_UpdateRects(mDrawingScreen, 1, &dest);
}

void Board::setScreen(SDL_Surface *screen)
{
	mDrawingScreen = screen;
}