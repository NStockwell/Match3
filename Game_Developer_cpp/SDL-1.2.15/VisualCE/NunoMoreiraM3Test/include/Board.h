
#ifndef BOARD_H
#define BOARD_H

//Number of sequential gems with the same type to be considered a match
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

//Struct to save the last 2 gems swapped so undo can be done if their new positions didn't generate any match
struct Undo{
	Point p1;
	Point p2;
};

//Struct with information on the number of gems matched in each column and the gem Y index of the deepest match
//this is used for later calculations and easier access to information
struct MatchingInfo
{
	int* numberMatchesInColumn;
	int* lowestGemPosition;
};


//Class with the information of the gems present on the game and how they should interact
class Board : public GraphicObject
{

public:
	//Constructor with the number of rows, columns, number of different types of gems, and size of a cell where gems will be drawn
	Board(int r, int c, int numGemTypes, Point gemSize);
	~Board();
	void update(float dt);
	void render();
	void init();
	void mouseOver(int x, int y);
	void mousePressed(int x, int y);
	void mouseReleased(int x, int y);

	//Goes to the position p.x, p.y and internally saves that gem as selected
	void selecteGemAtPoint(Point p);

	//true if gems are being moved around the board
	bool isAnimating();

private:
	//Number of rows and columns
	int mRows;
	int mColumns;

	
	//rows*columns
	int mTotalElements;

	
	//Number of different types of gems we have available
	int mNumGemTypes;

	
	//Time the movement of the gems takes in seconds
	float mAnimationTime;
	
	
	//Flag to signal if gems are  being moved around
	bool mAnimating;

	
	//Flag to signal if we are in the switching phase of the gems animation
	bool mSwitching;

	
	//Flag to signal if we should undo the last switch because it didn't generate any match
	bool mUndoSwitch;

	
	//Flag to signal if any matches were made in this switch
	bool mMatchesMade;


	//Flag to signal if we are currently in the process of initializing the board
	bool mInitializing;


	//Flag to signal if the mouse button is pressed
	bool mMousePressed;

	
	//Width and Height of a cell where the gems are to be placed
	Point mGemSize;

	
	//Structure holding the info on the last 2 gems switched
	Undo mUndo;

	
	//Structure with information on the last matches
	MatchingInfo mMatchingInfo;


	//Gems vector representing our board
	std::vector<Gem*> mTiles;

	
	//Current animations being made
	vector<GemAnimator*> mAnimations;


	//The x,y indexes on the board of the matches that have been generated (by switch or combo)
	vector<Point> mStoredMatches;

	
	//The x,y index of the currently selected Gem (-1,-1) if none
	Point mSelectedGemIndex;
	
	
	//Ends the initializing proccess
	void finishInitializing();

	
	//Returns true if the x,y position (screen coordinates, not indexes on the vector) is inside the board area (
	bool insideBoundaries(int x, int y);	
	inline bool insideBoundaries(Point p) {return insideBoundaries(p.getX(), p.getY());}

	
	//returns true if the x,y index is present in the board (> than -1,-1 and <= row,colums)
	bool existsIndex(int x, int y);
	inline bool existsIndex(Point p) {return existsIndex(p.getX(), p.getY());}

	
	//Returns the x,y index givent the screen coordinates
	Point coordinateToIndex(int x, int y);
	inline Point coordinateToIndex(Point p) {return coordinateToIndex(p.getX(), p.getY());}
	
	
	//Returns the index on the vector of the given x,y index of the cell
	int XYCoordinatesToIndex(int x, int y);
	inline int XYCoordinatesToIndex(Point p) { 	return XYCoordinatesToIndex(p.getX(), p.getY());}

	
	/**
	* switches 2 gems given their indexes.
	* @param Point g1 The index of the first gem
	* @param Point g2 The index of the second gem
	* @param reverting True if we are undoing or don't want to check for new matches after the switch is done
	* @param animate True if we want the animation to occur
	*/
	void switchGems(Point g1, Point g2, bool reverting = false, bool animate = true);
	
	/*
	* Ask the GemAnimator to prepare the animation for moving a Gem to a screen position
	* @param Gem* g the gem we want to move
	* @param Point p the screen position we want the gem to be moved to
	*/
	void moveGem(Gem* g, Point p);
	
	/**
	* Given an index p, checks recursively the neighbours to see if they have the same type. adds to the result when at least 3 are found
	* @param Point p, the index we want to start the search from
	*/
	vector<Point> checkForMatches(Point p);
	
	/**
	* Given a vector of matches, sorts them by ascending order and fills the MatchingInfo structure
	* @param vector<Point> matches the indexes of the gems that match
	*/
	void storeMatches(vector<Point> matches);

	/**
	* Replaces the matched gems, switches those and the gems above positions
	*/
	void makeMatches();
	
	/**
	* Taking account the info in MatchingInfo, checks the cells for new matches
	*/
	void checkCombos();
	
	/**
	* Checks in one direction if the gem is of the same type. If true, calls itself, otherwise returns (-1,-1)
	*/
	Point isUpNeighbourTheSameType(Point p);
	Point isDownNeighbourTheSameType(Point p);
	Point isLeftNeighbourTheSameType(Point p);
	Point isRightNeighbourTheSameType(Point p);
	

	/**
	* Given 2 points, checks if they both have the same type
	* @param current The current gem we are matching against
	* @param neighbour The neighbour gem we want to compare
	*/
	Point isNeighbourTheSameType(Point current, Point neighbour);


	/**
	* returns true if both gems are of the same type
	* @param Gem* g The first gem
	* @param Gem* g2 The second gem
	*/
	bool isSameType(Gem* g1, Gem* g2);


	/**
	* Merges two vectors with matches, ignoring duplicates
	* @param vector<Point> dest The final vector with merged matches
	* @param vector<Point> src The matches to copy from
	*/
	void mergeMatches(vector<Point> *dest, vector<Point> src);


	/**
	* cleans the MatchingInfo structure
	*/
	void clearNumberMatchesInColumns();
};

#endif