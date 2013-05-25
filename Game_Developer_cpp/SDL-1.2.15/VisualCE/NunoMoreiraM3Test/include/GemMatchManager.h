
#ifndef GEMMATCHMANAGER_H
#define GEMMATCHMANAGER_H

#include "Gem.h"
#include "Point.h"
#include "SDL.h"
#include "SDL_image.h"
#include <vector>

using namespace std;

class GemMatchManager{

public:
	static GemMatchManager& getInstance()
	{
		static GemMatchManager instance;
		return instance;
	}
	void setGemBoard(vector<Gem*> board, int rows, int columns);
	bool hasMatches();
	vector<Point> checkForMatches(Point p);

private:
	GemMatchManager() {};                  
    GemMatchManager(GemMatchManager const&);      // Don't Implement
    void operator=(GemMatchManager const&); // Don't implement

	vector<Gem*> mGems;
	int mRows, mColumns;
};

#endif