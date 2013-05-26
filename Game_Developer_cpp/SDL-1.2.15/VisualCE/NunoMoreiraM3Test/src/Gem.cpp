
#include "Gem.h"


Gem::Gem(int type, char* imageFile):GraphicObject(Point(0,0),imageFile)
{
	mType = type;
}

Gem::~Gem()
{
}

void Gem::setType(int type)
{
	mType = type;
}
int  Gem::getType()
{
	return mType;
}

void Gem::incPosition(float x, float y)
{
	mPosition.setX(mPosition.getX() + x);
	mPosition.setY(mPosition.getY() + y);
	dirty();
}


void Gem::render()
{
	
	if(!mInitialized || !mVisible/* || !mDirty*/)
		return;

	SDL_Rect dest;

	dest.x = mPosition.getX();
	dest.y = mPosition.getY();
	dest.w = mSize.getX();// mBackground->w;
	dest.h = mSize.getY();//mBackground->h;

	SDL_BlitSurface(mImage, NULL, mDrawingScreen, &dest);

	//SDL_UpdateRects(mDrawingScreen, 1, &dest);
	
	undirty();
}

void Gem::update(float dt)
{

}