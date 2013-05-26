
#include "Gem.h"


Gem::Gem(int type, char* imageFile)
{
	mPosition =  Point(0,0);
	mInitialized = false;
	mType = type;
	mDrawingScreen = NULL;
	
	mVisible = true;
	mDirty = true;
	mImageFilename = std::string(imageFile);
}

Gem::~Gem()
{
}


	void Gem::dirty()
	{
		mDirty = true;
	}
	void Gem::undirty()
	{
		mDirty = false;
	}

void Gem::setPosition(float x, float y)
{
	mPosition.setX(x);
	mPosition.setY(y);
	dirty();
	
}

void Gem::setType(int type)
{
	mType = type;
}
	
void Gem::setImageFile(char* filename)
{
	mImageFilename = std::string(filename);
	mBackground = IMG_Load(mImageFilename.c_str());
	if(mBackground == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", mImageFilename, SDL_GetError());
		return;
	}
	dirty();
}

void Gem::incPosition(float x, float y)
{
	mPosition.setX(mPosition.getX() + x);
	mPosition.setY(mPosition.getY() + y);
	dirty();
}

Point Gem::getPosition()
{
	return mPosition;
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
	dirty();
}

void Gem::setVisible(bool visible)
{
	mVisible = visible;
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
	if(!mInitialized || !mVisible/* || !mDirty*/)
		return;

	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = mBackground->w;
	dest.h = mBackground->h;

	SDL_BlitSurface(mBackground, NULL, mDrawingScreen, &dest);

	SDL_UpdateRects(mDrawingScreen, 1, &dest);
	
	undirty();

}

void Gem::update(float dt)
{

}