
#ifndef BOARD_H
#define BOARD_H

#define MIN_MATCH_SIZE 3

#include "Gem.h"
#include "Point.h"
#include "GemManager.h"
#include "GemAnimator.h"


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

class Board
{

public:
	Board(int r, int c, /*char* backgroundFile,*/ int numGemTypes, Point gemSize);
	~Board();
	void setScreen(SDL_Surface *screen);
	void update(float dt);
	void render();
	void render(int x, int y);
	void init();
	void mouseOver(int x, int y);
	void mousePressed(int x, int y);
	void setPosition(int x, int y);
	bool isAnimating();

private:
	int mRows;
	int mColumns;
	int mTotalElements;
	int mNumGemTypes;
	bool mInitialized;
	bool mAnimating;
	bool mSwitching;
	bool mUndoSwitch;
	Point mPosition;
	Point mSize;
	Point mGemSize;
	Undo mUndo;
	MatchingInfo mMatchingInfo;
	/*std::string mBackgroundFileName;*/
	/*SDL_Surface *mBackground;*/
	SDL_Surface *mDrawingScreen;
	std::vector<Gem*> mTiles;
	vector<GemAnimator*> mAnimations;
	vector<Point> mStoredMatches;
	Point mSelectedGemIndex;


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
	

	bool hasMatches();
	vector<Point> checkForMatches(Point p);
	void storeMatches(vector<Point> matches);
	void makeMatches();
	vector<Point> getNeighbours(Point p);

	Point isUpNeighbourTheSameType(Point p);
	Point isDownNeighbourTheSameType(Point p);
	Point isLeftNeighbourTheSameType(Point p);
	Point isRightNeighbourTheSameType(Point p);

	Point isNeighbourTheSameType(Point current, Point neighbour);

	bool isSameType(Gem* g1, Gem* g2);


	
	void clearNumberMatchesInColumns();
};

#endif