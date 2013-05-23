
#ifndef GEM_H
#define GEM_H

#include "SDL.h"
#include "SDL_image.h"

#include <string>

class Gem
{

public:
	Gem(int type, char* imageFile);
	~Gem();
	void init();
	void setScreen(SDL_Surface *screen);
	void render(int x, int y);
	void update(float dt);

private:
	bool mInitialized;
	int mType;
	std::string mImageFile;
	SDL_Surface *mBackground;
	SDL_Surface *mDrawingScreen;

};

#endif