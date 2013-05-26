
#ifndef BOARD_H
#define BOARD_H

#define MIN_MATCH_SIZE 3

#include "Gem.h"
#include "Point.h"
#include "GemManager.h"
#include "GemAnimator.h"
#include "GraphicObject.h"

#include "SDL.h"
#include "SDL_image.h"

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;
struct Undo{
	Point p1;
	Point p2;
};

struct MatchingInfo
{
	int* numberMatchesInColumn;
	int* lowestGemPosition;
};

enum AnimationState
{
	pullingCellsDown
};

class Board : public GraphicObject
{

public:
	Board(int r, int c, int numGemTypes, Point gemSize);
	~Board();
	void update(float dt);
	void render();
	void init();
	void mouseOver(int x, int y);
	void mousePressed(int x, int y);
	void mouseReleased(int x, int y);

	
	void selecteGemAtPoint(Point p);

	bool isAnimating();

private:
	int mRows;
	int mColumns;
	int mTotalElements;
	int mNumGemTypes;
	float mAnimationTime;
	
	bool mAnimating;
	bool mSwitching;
	bool mUndoSwitch;
	bool mMatchesMade;
	bool mInitializing;
	bool mMousePressed;

	Point mGemSize;
	Undo mUndo;
	MatchingInfo mMatchingInfo;

	std::vector<Gem*> mTiles;
	vector<GemAnimator*> mAnimations;
	vector<Point> mStoredMatches;
	Point mSelectedGemIndex;
	
	void finishInitializing();

	bool insideBoundaries(int x, int y);	
	bool insideBoundaries(Point p);

	bool existsIndex(Point p);
	bool existsIndex(int x, int y);
	Point coordinateToIndex(int x, int y);
	Point coordinateToIndex(Point p);
	int XYCoordinatesToIndex(Point p);
	int XYCoordinatesToIndex(int x, int y);
	
	void switchGems(Point g1, Point g2, bool reverting = false, bool animate = true);
	void moveGem(Gem* g, Point p);
	

	vector<Point> checkForMatches(Point p);
	void storeMatches(vector<Point> matches);
	void makeMatches();
	void checkCombos();
	
	Point isUpNeighbourTheSameType(Point p);
	Point isDownNeighbourTheSameType(Point p);
	Point isLeftNeighbourTheSameType(Point p);
	Point isRightNeighbourTheSameType(Point p);

	Point isNeighbourTheSameType(Point current, Point neighbour);

	bool isSameType(Gem* g1, Gem* g2);


	
	void mergeMatches(vector<Point> *dest, vector<Point> src);
	void clearNumberMatchesInColumns();
};

#endif