
#ifndef GEM_H
#define GEM_H

#include "SDL.h"
#include "SDL_image.h"

#include "Point.h"

#include <string>

class Gem
{

public:
	Gem(int type, char* imageFile);
	~Gem();
	void init();
	void setScreen(SDL_Surface *screen);
	
	void render();
	void render(int x, int y);
	void update(float dt);
	
	void mouseOver(bool over);
	void setPosition(int x, int y);

private:
	Point mPosition;
	bool mInitialized;
	bool mVisible;
	int mType;
	std::string mImageFile;
	SDL_Surface *mBackground;
	SDL_Surface *mDrawingScreen;

};

#endif