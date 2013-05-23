
#include "Gem.h"


Gem::Gem(Point pos, int type, char* imageFile)
{
	mPosition = pos;
	mInitialized = false;
	mType = type;
	mDrawingScreen = NULL;
	
	mVisible = true;

	mImageFile = std::string(imageFile);
}

Gem::~Gem()
{
}

void Gem::init()
{
	mBackground = IMG_Load(mImageFile.c_str());
	if(mBackground == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", mImageFile, SDL_GetError());
		return;
	}

	mInitialized = true;
}

void Gem::setScreen(SDL_Surface *screen)
{
	mDrawingScreen = screen;
}

void Gem::mouseOver(bool over)
{
	mVisible = !over;
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