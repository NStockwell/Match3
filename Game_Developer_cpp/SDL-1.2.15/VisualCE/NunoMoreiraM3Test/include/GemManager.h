
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

	//add a gem to the vector of available types of gems
	int addGem(int type, char* filename);

	/**
	* Returns the pointer to a new Gem with the type provided
	* @param int type The type of the gem
	*/
	Gem* getGem(int type);

	/**
	* Returns a pointer to a Gem with a random type
	*/
	Gem* getRandomTypeGem();
	
	/**
	* Modifies the provided gem to a specific type
	* @param Gem* g The gem to modify 
	* @param int type The new type for the gem
	*/
	void changeGemType(Gem* g, int type);
	
	/**
	* Modifies the provided gem to a random type
	* @param Gem* g The gem to modify
	*/
	void changeGemToRandomType(Gem* g);


private:
	GemManager() {srand (time(NULL));};                  
    GemManager(GemManager const&);      // Don't Implement
    void operator=(GemManager const&); // Don't implement

	//Available Gem types
	vector<Gem> mGems;

};

#endif