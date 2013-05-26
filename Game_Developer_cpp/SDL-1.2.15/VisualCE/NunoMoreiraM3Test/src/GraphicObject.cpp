	
#include "GraphicObject.h"

GraphicObject::GraphicObject()
{
	mPosition = Point(0,0);
	mSize = Point(0,0);
	mInitialized = false;
	mVisible = false;
	mDirty = false;
	mImageFileName = "";
	mImage = NULL;
	mDrawingScreen = NULL;
}
GraphicObject::GraphicObject(Point pos, char* fileName)
{
	mPosition = pos;
	mSize = Point(0,0);
	mInitialized = false;
	mVisible = false;
	mDirty = false;
	mImageFileName = fileName;
	mImage = NULL;
	mDrawingScreen = NULL;
}
	GraphicObject::~GraphicObject()
	{
		SDL_FreeSurface(mImage);
	}

	void GraphicObject::init()
	{
		setImage((char*)mImageFileName.c_str());
		mInitialized = true;
	}
	void GraphicObject::update(float dt)
	{
		if(!mInitialized)
			return;
	}
	void GraphicObject::render()
	{
		if(!mInitialized)
			return;

		if(!mDirty)
			return;

		undirty();
	}
	void GraphicObject::setScreen(SDL_Surface *screen)
	{
		mDrawingScreen = screen;
		dirty();
	}

	Point GraphicObject::getPosition()
	{
		return mPosition;
	}
	void GraphicObject::setPosition(Point p)
	{
		mPosition = p;
		dirty();
	}

	Point GraphicObject::getSize()
	{
		return mSize;
	}
	void GraphicObject::setSize(Point p)
	{
		mSize = p;
		dirty();
	}

	void GraphicObject::setImage(char* filename)
	{
		mImageFileName = string(filename);
		mImage = IMG_Load(mImageFileName.c_str());
		if(mImageFileName == "")
		{
			fprintf(stderr, "No image\n");// Couldn't load %s: %s\n");
			return;
		}
		if(mImage == NULL)
		{
			fprintf(stderr, "Couldn't load %s: %s\n", mImageFileName, SDL_GetError());
			return;
		}
		setSize(Point(mImage->w, mImage->h));
		dirty();
	}
	string  GraphicObject::getImageFilename()
	{
		return mImageFileName;
	}

	void GraphicObject::setVisible(bool visible)
	{
		mVisible = visible;
		dirty();
	}
	bool GraphicObject::getVisible()
	{
		return mVisible;
	}
	void GraphicObject::dirty()
	{
		mDirty = true;
	}
	bool GraphicObject::getDirty()
	{
		return mDirty;
	}
	void GraphicObject::undirty()
	{
		mDirty = false;
	}

	