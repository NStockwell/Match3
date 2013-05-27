
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
	GraphicObject::render();
	//if(!mInitialized || !mVisible/* || !mDirty*/)
	//	return;

	//SDL_Rect dest;

	//dest.x = mPosition.getX();
	//dest.y = mPosition.getY();
	//dest.w = mSize.getX();
	//dest.h = mSize.getY();

	//SDL_BlitSurface(mImage, NULL, mDrawingScreen, &dest);

	//undirty();
}

void Gem::update(float dt)
{

}