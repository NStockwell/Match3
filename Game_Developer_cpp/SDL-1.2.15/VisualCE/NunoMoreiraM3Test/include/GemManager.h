
#ifndef GEMMANAGER_H
#define GEMMANAGER_H

#include "Gem.h"
#include "SDL.h"
#include "SDL_image.h"
#include <vector>

#include <stdlib.h>
#include <time.h>

using namespace std;

class GemManager{

public:
	static GemManager& getInstance()
	{
		static GemManager instance;
		return instance;
	}

	int addGem(int type, char* filename);
	Gem* getGem(int type);
	Gem* getRandomTypeGem();


private:
	GemManager() {srand (time(NULL));};                  
    GemManager(GemManager const&);      // Don't Implement
    void operator=(GemManager const&); // Don't implement

	vector<Gem> mGems;

};

#endif