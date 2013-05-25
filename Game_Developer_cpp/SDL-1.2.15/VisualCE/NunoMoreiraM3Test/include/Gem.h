
#ifndef GEM_H
#define GEM_H

#include "SDL.h"
#include "SDL_image.h"

#include "Point.h"

#include <string>

using namespace std;

class Gem
{

public:
	Gem(int type, char* imageFilename);
	~Gem();
	void init();
	void setScreen(SDL_Surface *screen);
	void setVisible(bool visible);

	void render();
	void render(int x, int y);
	void update(float dt);
	
	void mouseOver(bool over);
	void mousePressed(bool pressed);
	void setPosition(float x, float y);
	void incPosition(float x, float y);
	Point getPosition();
	int getType();
	string getImagefilename();

private:
	Point mPosition;
	bool mInitialized;
	bool mVisible;
	bool mDirty;
	int mType;
	std::string mImageFilename;
	SDL_Surface *mBackground;
	SDL_Surface *mDrawingScreen;


	void dirty();
	void undirty();
};

#endif