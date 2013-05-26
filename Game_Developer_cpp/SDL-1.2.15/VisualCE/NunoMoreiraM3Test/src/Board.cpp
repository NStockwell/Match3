
#include "Board.h"


Board::Board(int rows, int columns, int numGemTypes, Point gemSize):GraphicObject()
{
	mAnimationTime = 0.01;

	mRows = rows;
	mColumns = columns;

	mTotalElements = rows*columns;

	mNumGemTypes = numGemTypes;

	mTiles = std::vector<Gem*>(mTotalElements);
	
	mSwitching = false;
	mUndoSwitch = false;
	mAnimating = false;
	mMatchesMade = false;
	mGemSize = gemSize;

	setSize( Point(mColumns * mGemSize.getX(), mRows * mGemSize.getY()) );


	mSelectedGemIndex = NULL_POINT;
	
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

	GraphicObject::init();

	mMatchingInfo.numberMatchesInColumn = new int[mColumns];
	mMatchingInfo.lowestGemPosition = new int[mColumns];

	
	for(int i = 0; i < mTotalElements; i++)
	{
		
		//int type = rand() % mNumGemTypes;
		mTiles[i] = GemManager::getInstance().getRandomTypeGem();//new Gem(type, (char*)gemsAssets.at(type).c_str());
		mTiles[i]->setPosition( Point(mPosition.getX() + (i%mColumns) * mGemSize.getX(), mPosition.getY() + (i/mRows) * mGemSize.getY()) );
		mTiles[i]->setScreen(mDrawingScreen);
		mTiles[i]->init();
	}


	for(int i = 0; i < mColumns; i++)
	{
		mMatchingInfo.numberMatchesInColumn[i] = 1;
		mMatchingInfo.lowestGemPosition[i] = mRows-1;
	}
	checkCombos();
	makeMatches();

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

void Board::mergeMatches(vector<Point> *dest, vector<Point> src)
{
	for(int i = 0; i < src.size(); i++)
		{
			vector<Point>::iterator it = std::find(dest->begin(), dest->end(), src.at(i));
			if(it == dest->end())
				dest->push_back(src.at(i));
		}
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

		mergeMatches(&matches, matchesP2);

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
	{
		mAnimationTime = 0.4;
		mMatchesMade = false;
		return;
	}
	mMatchesMade = true;
	for(int i = 0; i < mStoredMatches.size(); i++)
	{
		Point matchedGemPoint = mStoredMatches.at(i);
		Gem* g = mTiles[XYCoordinatesToIndex(matchedGemPoint)];
		g->setVisible(false);
		GemManager::getInstance().changeGemToRandomType(g);
	}

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

	mStoredMatches.clear();
}

void Board::checkCombos()
{
	mMatchesMade = false;
	vector<Point> comboMatches;
	for(int i = 0; i < mColumns; i++)
	{
		if(mMatchingInfo.numberMatchesInColumn[i] == 0)
			continue;

		for(int j = 0; j <= mMatchingInfo.lowestGemPosition[i]; j++)
		{
			Point modifiedGemPoint = Point(i,j);// mStoredMatches.at(i);
			Gem* g = mTiles[XYCoordinatesToIndex(modifiedGemPoint)];
			g->setVisible(true);
			mergeMatches(&comboMatches, checkForMatches(modifiedGemPoint ) );
		}
	}
	
	clearNumberMatchesInColumns();
	
	if(comboMatches.size() > 0)
		storeMatches(comboMatches);
}

void Board::moveGem(Gem* g, Point p)
{
	GemAnimator* ga1 = new GemAnimator();
	ga1->moveTo(g,p,mAnimationTime);
	mAnimations.push_back(ga1);
}

void Board::mouseOver(int x, int y)
{
	if(!insideBoundaries(x,y) || mSwitching)
		return;
	
	Point index = coordinateToIndex(x,y);
	
	mTiles[index.getX() + index.getY()*mColumns]->mouseOver(x,y); 
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
		mSelectedGemIndex = NULL_POINT;
	}
	else
	{
		mSelectedGemIndex = xy;// = mTiles[index];
	}
	mTiles[index]->mousePressed(x,y); 
}
void Board::update(float dt)
{
	GraphicObject::update(dt);
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
		if(mMatchesMade)
		{
			checkCombos();
		}

		makeMatches();
	}
}

void Board::render()
{	
	GraphicObject::render();
	if(!mInitialized)
		return;

	for(int i = 0; i < mTotalElements; i++)
	{
		mTiles[i]->render( ); 
	}
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
	return NULL_POINT;// Point(-1,-1);
}


Point Board::isUpNeighbourTheSameType(Point p)
{
	Point pUp = p + Point(0, -1);
	return isNeighbourTheSameType(p, pUp);
}
Point Board::isDownNeighbourTheSameType(Point p)
{
	Point pDown = p + Point(0, 1) ;
	return isNeighbourTheSameType(p, pDown);
}
Point Board::isLeftNeighbourTheSameType(Point p)
{
	Point pLeft = p + Point(-1,0);
	return isNeighbourTheSameType(p, pLeft);
}
Point Board::isRightNeighbourTheSameType(Point p)
{
	Point pRight = p + Point(1,0);
	return isNeighbourTheSameType(p, pRight);
}

bool Board::isSameType(Gem* g1, Gem* g2)
{
	return g1->getType() == g2->getType();
}

