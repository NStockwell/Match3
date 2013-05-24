
#include "Gem.h"


Gem::Gem(int type, char* imageFile)
{
	mPosition =  Point(0,0);
	mInitialized = false;
	mType = type;
	mDrawingScreen = NULL;
	
	mVisible = true;

	mImageFilename = std::string(imageFile);
}

Gem::~Gem()
{
}

void Gem::setPosition(int x, int y)
{
	mPosition.setX(x);
	mPosition.setY(y);
}

void Gem::init()
{
	mBackground = IMG_Load(mImageFilename.c_str());
	if(mBackground == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", mImageFilename, SDL_GetError());
		return;
	}

	mInitialized = true;
}

void Gem::setScreen(SDL_Surface *screen)
{
	mDrawingScreen = screen;
}


int  Gem::getType()
{
	return mType;
}

string  Gem::getImagefilename()
{
	return mImageFilename;
}

void Gem::mousePressed(bool pressed)
{

}

void Gem::mouseOver(bool over)
{
	//mVisible = !over;
}

void Gem::render()
{
	render(mPosition.getX(), mPosition.getY());
}

void Gem::render(int x, int y)
{
	if(!mInitialized || !mVisible)
		return;

	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = mBackground->w;
	dest.h = mBackground->h;

	SDL_BlitSurface(mBackground, NULL, mDrawingScreen, &dest);

	SDL_UpdateRects(mDrawingScreen, 1, &dest);
}

void Gem::update(float dt)
{

}