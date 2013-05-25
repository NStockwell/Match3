
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
	mUndoSwitch = false;
	mAnimating = false;
	mGemSize = gemSize;

	mSize.setX(mColumns * mGemSize.getX());
	mSize.setY(mRows * mGemSize.getY());

	mSelectedGemIndex = NULL_POINT;
	//mSelectedGemIndex.setX(-1);// = NULL;
	//mSelectedGemIndex.setY(-1);
	
}

Board::~Board()
{
	delete mMatchingInfo.numberMatchesInColumn;
	delete mMatchingInfo.lowestGemPosition;

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

	mMatchingInfo.numberMatchesInColumn = new int[mColumns];
	mMatchingInfo.lowestGemPosition = new int[mColumns];

	
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

bool Board::isAnimating()
{
	return mAnimating;
}

bool Board::insideBoundaries(Point p)
{
	return insideBoundaries(p.getX(), p.getY());
}

bool Board::insideBoundaries(int x, int y)
{
	if(x >= mSize.getX() + mPosition.getX()  || y >= mSize.getY() + mPosition.getY() || x < mPosition.getX() || y < mPosition.getY())
		return false;
	return true;
}


	bool Board::existsIndex(Point p)
	{
		return existsIndex(p.getX(), p.getY());
	}

	bool Board::existsIndex(int x, int y)
	{
		return (x >=0 && x < mColumns && y>= 0 && y < mRows);
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

Point Board::coordinateToIndex(Point p)
{
	return coordinateToIndex(p.getX(), p.getY());
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

	Point distance = mSelectedGemIndex - xy;

	if(mSelectedGemIndex != NULL_POINT && distance.getLength() == 1)
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

void Board::switchGems(Point p1, Point p2, bool reverting, bool animate)
{
	mSwitching = true;
	int p1Index = XYCoordinatesToIndex(p1);
	int p2Index = XYCoordinatesToIndex(p2);
	Gem* tempGem = mTiles[p1Index];
	mTiles[p1Index] = mTiles[p2Index];
	mTiles[p2Index] = tempGem;
	
	if(animate)
	{
		moveGem(mTiles[p1Index], mTiles[p2Index]->getPosition());
		moveGem(mTiles[p2Index], mTiles[p1Index]->getPosition());
	}
	if(!reverting)
	{
		vector<Point> matches = checkForMatches(p1);	
		vector<Point> matchesP2 = checkForMatches(p2);

		for(int i = 0; i < matchesP2.size(); i++)
		{
			vector<Point>::iterator it = std::find(matches.begin(), matches.end(), matchesP2.at(i));
			if(it == matches.end())
				matches.push_back(matchesP2.at(i));
		}

		if(matches.size() == 0)
		{
			mUndoSwitch = true;
			mUndo.p1 = p1;
			mUndo.p2 = p2;
		}
		else
		{
			storeMatches(matches);
		}
	}

	mSwitching = false;
}

void Board::storeMatches(vector<Point> matches)
{
	clearNumberMatchesInColumns();
	for(int i = 0; i < matches.size() ; i++)
    {
		Point checkForMinimumPoint = matches.at(i);
		mMatchingInfo.numberMatchesInColumn[(int)checkForMinimumPoint.getX()]++;
		int minimumPointY = checkForMinimumPoint.getY();
		if(mMatchingInfo.lowestGemPosition[(int)checkForMinimumPoint.getX()] < minimumPointY)
		{
			mMatchingInfo.lowestGemPosition[(int)checkForMinimumPoint.getX()] = minimumPointY;
		}
	}
	for(int i = 0; i < matches.size() ; i++)
    {
        for (int j=0; j<matches.size()-i-1;j++)
        {
			Point current = matches.at(j);
			Point next = matches.at(j+1);
			Point temp;
			if(current < next)
			{
				temp = current;
				matches[j] = next;
				matches[j+1] = temp;
			}
        }
    }

	mStoredMatches = matches;
}

void Board::clearNumberMatchesInColumns()
{
	for(int i = 0; i < mColumns; i++)
	{
		mMatchingInfo.numberMatchesInColumn[i] = 0;
		mMatchingInfo.lowestGemPosition[i] = 0;
	}
}

void Board::makeMatches()
{
	Point p;
	if(mStoredMatches.empty())
		return;

	for(int i = 0; i < mColumns; i++)
	{
		int xIndex = i;
		for(int k = 0; k < mMatchingInfo.numberMatchesInColumn[xIndex]; k++)
			for(int j = mMatchingInfo.lowestGemPosition[xIndex] -1; j >=0; j--)
			{
				switchGems(Point(xIndex, j), Point(xIndex, j+1), true,false);
			}
	}
	//Now all matched gems should be at the top
	for(int i = 0; i < mColumns; i++)
	{
		int xIndex = i;
		int occurrencesInColumn = mMatchingInfo.numberMatchesInColumn[xIndex];
		
		for(int j =  mMatchingInfo.lowestGemPosition[xIndex]; j >= 0; j--)
		{
			
			moveGem(mTiles.at(XYCoordinatesToIndex(xIndex,j)) ,
				(mTiles.at(XYCoordinatesToIndex(xIndex,(j + occurrencesInColumn) % (mMatchingInfo.lowestGemPosition[xIndex] +1)))->getPosition() ));
		}
	}
	clearNumberMatchesInColumns();
	mStoredMatches.clear();
}

void Board::moveGem(Gem* g, Point p)
{
	GemAnimator* ga1 = new GemAnimator();
	ga1->moveTo(g,p,0.25);
	mAnimations.push_back(ga1);
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

	vector<int> finishedAnimationsIndexes;
	mAnimating = false;
	for(int i = 0; i < mAnimations.size(); i++)
	{
		mAnimating = true;
		mAnimations.at(i)->update(dt);
		if(mAnimations.at(i)->getFinished())
		{
			finishedAnimationsIndexes.push_back(i);
		}
	}

	//start from end so indexes on mAnimations don't change when deleting
	for(int i = finishedAnimationsIndexes.size() -1; i >=0; i--)
	{
		mAnimations.erase(mAnimations.begin() + finishedAnimationsIndexes.at(i));
	}
	if(mAnimations.size() == 0 && mUndoSwitch)
	{
		switchGems(mUndo.p1, mUndo.p2, true);
		mUndoSwitch = false;
	}
	if(mAnimations.size() == 0 && finishedAnimationsIndexes.size() != 0)
	{
		makeMatches();
	}


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



bool Board::hasMatches()
{
	return false;
}

vector<Point> Board::checkForMatches(Point p)
{
	vector<Point> result;
	Gem* movedGem = mTiles[XYCoordinatesToIndex(p)];
	Gem* iterGem = movedGem;
	Point checkingThisPoint = p;

	/**
	*	VERTICAL MATCHES
	**/
	vector<Point> verticalMatches;
	while( (checkingThisPoint = isUpNeighbourTheSameType(checkingThisPoint)) != NULL_POINT)
	{
		verticalMatches.push_back(checkingThisPoint);
	}
	checkingThisPoint = p;
	while( (checkingThisPoint = isDownNeighbourTheSameType(checkingThisPoint)) != NULL_POINT)
	{
		verticalMatches.push_back(checkingThisPoint);
	}
	// MIN_MATCH_SIZE -1 because the moved gem isn't being added yet
	if(verticalMatches.size() >= MIN_MATCH_SIZE -1)
	{
		for(int i = 0; i < verticalMatches.size(); i++)
		{
			result.push_back(verticalMatches[i]);
		}
	}

	/**
	*	HORIZONTAL MATCHES
	**/
	vector<Point> horizontalMatches;
	checkingThisPoint = p;
	
	while( (checkingThisPoint = isLeftNeighbourTheSameType(checkingThisPoint)) != NULL_POINT)
	{
		horizontalMatches.push_back(checkingThisPoint);
	}
	checkingThisPoint = p;
	while( (checkingThisPoint = isRightNeighbourTheSameType(checkingThisPoint)) != NULL_POINT)
	{
		horizontalMatches.push_back(checkingThisPoint);
	}
	// MIN_MATCH_SIZE -1 because the moved gem isn't being added yet
	if(horizontalMatches.size() >= MIN_MATCH_SIZE -1)
	{
		for(int i = 0; i < horizontalMatches.size(); i++)
		{
			result.push_back(horizontalMatches[i]);
		}
	}

	/**
	*	ADDING SELF IF MATCHES FOUND
	**/
	if(result.size() > 0)
	{
		result.push_back(p);    
	}


	return result;
}


Point Board::isNeighbourTheSameType(Point current, Point neighbour)
{
	if(existsIndex(neighbour))
	{
		Gem* currentGem = mTiles[XYCoordinatesToIndex(current)];
		Gem* upperGem = mTiles[XYCoordinatesToIndex(neighbour)];
		if(isSameType(currentGem,upperGem))
			return neighbour;
	}
	return Point(-1,-1);
}


Point Board::isUpNeighbourTheSameType(Point p)
{
	Point pUp = p + Point(0, -1);//  Point( p.getX(), p.getY()-1);
	return isNeighbourTheSameType(p, pUp);
}
Point Board::isDownNeighbourTheSameType(Point p)
{
	Point pDown = p + Point(0, 1) ;// Point(p.getX(), p.getY() +1);
	return isNeighbourTheSameType(p, pDown);
}
Point Board::isLeftNeighbourTheSameType(Point p)
{
	Point pLeft = p + Point(-1,0);//Point(p.getX()-1, p.getY());
	return isNeighbourTheSameType(p, pLeft);
}
Point Board::isRightNeighbourTheSameType(Point p)
{
	Point pRight = p + Point(1,0);//Point(p.getX()+1, p.getY());
	return isNeighbourTheSameType(p, pRight);
}

bool Board::isSameType(Gem* g1, Gem* g2)
{
	return g1->getType() == g2->getType();
}

vector<Point> Board::getNeighbours(Point p)
{
	vector<Point> neighbours;

	Point pLeft = Point(p.getX() -1, p.getY());
	Point pRight = Point(p.getX() +1, p.getY());
	Point pUp = Point(p.getX(), p.getY() -1);
	Point pDown = Point(p.getX(), p.getY() +1);

	if(existsIndex(pLeft))
		neighbours.push_back(pLeft);

	if(existsIndex(pRight))
		neighbours.push_back(pRight);

	if(existsIndex(pUp))
		neighbours.push_back(pUp);

	if(existsIndex(pDown))
		neighbours.push_back(pDown);

	return neighbours;
}