
#include "Board.h"


Board::Board(int rows, int columns, int numGemTypes, Point gemSize):GraphicObject()
{
	mInitializing = false;
	//initializing with small value for quick matching during initialization process
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

	mMousePressed = false;


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
	GraphicObject::init();
	mInitializing = true;

	//set size according to rows, columns and size of cells
	setSize( Point(mColumns * mGemSize.getX(), mRows * mGemSize.getY()) );

	//initialize matching info
	mMatchingInfo.numberMatchesInColumn = new int[mColumns];
	mMatchingInfo.lowestGemPosition = new int[mColumns];

	//fill the board with random gems
	for(int i = 0; i < mTotalElements; i++)
	{
		mTiles[i] = GemManager::getInstance().getRandomTypeGem();
		mTiles[i]->setPosition( Point(mPosition.getX() + (i%mColumns) * mGemSize.getX(), mPosition.getY() + (i/mRows) * mGemSize.getY()) );
		mTiles[i]->setScreen(mDrawingScreen);
		mTiles[i]->init();
	}

	//fill the matching info with the info necessary to clear initial matches caused by the randomness
	for(int i = 0; i < mColumns; i++)
	{
		mMatchingInfo.numberMatchesInColumn[i] = 1;
		mMatchingInfo.lowestGemPosition[i] = mRows-1;
	}
	//check for matches and clean them 
	checkCombos();
	makeMatches();

	mInitialized = true;
}

void Board::finishInitializing()
{
	//reset the timer to a more visible value
	mAnimationTime = 0.4;
	mInitializing = false;
	for(int i = 0; i < mTotalElements; i++)
	{
		mTiles.at(i)->setVisible(true);
	}
}

bool Board::isAnimating()
{
	return mAnimating;
}


bool Board::insideBoundaries(int x, int y)
{
	if(x >= mSize.getX() + mPosition.getX()  || y >= mSize.getY() + mPosition.getY() || x < mPosition.getX() || y < mPosition.getY())
		return false;
	return true;
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


int Board::XYCoordinatesToIndex(int x, int y)
{
	return x + y*mColumns;
}

void Board::mergeMatches(vector<Point> *dest, vector<Point> src)
{
	//iterate on src, add to dest if it's not a duplicate
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
	//get vector index from coordinate in "matrix"
	int p1Index = XYCoordinatesToIndex(p1);
	int p2Index = XYCoordinatesToIndex(p2);
	//switch pointers
	Gem* tempGem = mTiles[p1Index];
	mTiles[p1Index] = mTiles[p2Index];
	mTiles[p2Index] = tempGem;
	
	//if we ante animations
	if(animate)
	{
		moveGem(mTiles[p1Index], mTiles[p2Index]->getPosition());
		moveGem(mTiles[p2Index], mTiles[p1Index]->getPosition());
	}

	//if we aren't reverting, check for matches
	if(!reverting)
	{
		vector<Point> matches = checkForMatches(p1);	
		vector<Point> matchesP2 = checkForMatches(p2);

		mergeMatches(&matches, matchesP2);

		//we weren't reverting and didn't find any match so need to undo the switch
		if(matches.size() == 0)
		{
			mUndoSwitch = true;
			mUndo.p1 = p1;
			mUndo.p2 = p2;
		}
		else
		{//we found matches so prepare things for animations
			storeMatches(matches);
		}
	}

	mSwitching = false;
}

void Board::storeMatches(vector<Point> matches)
{
	clearNumberMatchesInColumns();
	//save the number of gems matched in each column and the row of the deepest one
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
	//bubblesort the list, order is important when pulling the gems up to assure the empties are at the top
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
		mMatchesMade = false;
		return;
	}
	mMatchesMade = true;
	//hide matches and "create" a new gem
	for(int i = 0; i < mStoredMatches.size(); i++)
	{
		Point matchedGemPoint = mStoredMatches.at(i);
		Gem* g = mTiles[XYCoordinatesToIndex(matchedGemPoint)];
		//printf("%f %f ",matchedGemPoint.getX(), matchedGemPoint.getY());
		GemManager::getInstance().changeGemToRandomType(g);
	}
	//for every column, start at the deepest match made and pull that up, repeat the number of times matches were made in that column
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
	/* A					B
	*  B    should now be   C  assuming a 3 matches vertical (this is why bubblesort was important on storematches)
	*  C					D  this allows the modulo math to work and keep animations correct
	*  D					A
	*/

	//since all gems have been switched, time to calculate animations
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
	{//if no matches were made in this column, ignore it
		if(mMatchingInfo.numberMatchesInColumn[i] == 0)
			continue;

		//checkmatches on the all the elements of this column above the lowest gem matched
		for(int j = 0; j <= mMatchingInfo.lowestGemPosition[i]; j++)
		{
			Point modifiedGemPoint = Point(i,j);
			Gem* g = mTiles[XYCoordinatesToIndex(modifiedGemPoint)];
			g->setVisible(true);

			mergeMatches(&comboMatches, checkForMatches(modifiedGemPoint ) );
		}
	}
	
	//clear matchingInfo
	clearNumberMatchesInColumns();
	
	//if we have matches, store and save info
	if(comboMatches.size() > 0)
		storeMatches(comboMatches);
	else
		if(mInitializing)	//if we are still initializing but haven't any more matches, finish initialization
			finishInitializing();
}

void Board::moveGem(Gem* g, Point p)
{
	//create another animator and add it to the list of running animations
	GemAnimator* ga1 = new GemAnimator();
	ga1->moveTo(g,p,mAnimationTime);
	mAnimations.push_back(ga1);
	if(p.getY() < g->getPosition().getY() && mMatchesMade)
	{//hide the gems that are going upwards
		g->setVisible(false);
	}
}

void Board::mouseOver(int x, int y)
{
	//check if inside boundaries or able to interact
	if(!insideBoundaries(x,y) || mSwitching || !mMousePressed)
		return;
	
	//transform screen coordinates to index on vector
	Point xy = coordinateToIndex(x,y);
	int index = XYCoordinatesToIndex(xy);

	//select it
	selecteGemAtPoint(xy);
	
	mTiles[index]->mouseOver(x,y); 

}
void Board::mousePressed(int x, int y)
{
	if(!insideBoundaries(x,y) || mSwitching)
		return;
	
	mMousePressed = true;
	
	//transform screen coordinates to index on vector
	Point xy = coordinateToIndex(x,y);
	int index = XYCoordinatesToIndex(xy);
	
	//select it
	selecteGemAtPoint(xy);
	
	mTiles[index]->mousePressed(x,y); 
	
}
void Board::mouseReleased(int x, int y)
{
	mMousePressed = false;
}

void Board::selecteGemAtPoint(Point xy)
{
	int index = XYCoordinatesToIndex(xy);

	Point distance = mSelectedGemIndex - xy;
	//only allow if it's a neighbour and we have not yet selected any gem
	if(mSelectedGemIndex != NULL_POINT && distance.getLength() == 1)
	{
		switchGems(mSelectedGemIndex, xy);
		mSelectedGemIndex = NULL_POINT;
		mMousePressed = false;
	}
	else
	{
		mSelectedGemIndex = xy;// = mTiles[index];
	}
}

void Board::update(float dt)
{
	GraphicObject::update(dt);
	if(!mInitialized)
		return;

	vector<int> finishedAnimationsIndexes;
	mAnimating = false;
	for(int i = 0; i < mAnimations.size(); i++)
	{//we have animations, update them and if they are finished, add to finishedanimations to remove from list
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
	//we switched gems, animation is complete but no matches were made, undo
	if(mAnimations.size() == 0 && mUndoSwitch)
	{
		switchGems(mUndo.p1, mUndo.p2, true);
		mUndoSwitch = false;
	}
	//we made matches, check for combos
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
	//GraphicObject::render();
	if(!mInitialized || mInitializing)
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

