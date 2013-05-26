
#ifndef GEM_H
#define GEM_H

#include "SDL.h"
#include "SDL_image.h"

#include "GraphicObject.h"
#include "Point.h"

#include <string>

using namespace std;

class Gem : public GraphicObject
{

public:
	Gem(int type, char* imageFilename);
	~Gem();
	
	void render();
	void update(float dt);
	
	void incPosition(float x, float y);
	
	void setType(int type);
	int getType();

private:
	
	int mType;
	
};

#endif