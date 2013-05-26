
#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H

#include "Point.h"
#include "SDL.h"
#include "SDL_image.h"


#include <string>

using namespace std;

class GraphicObject
{
public:
	
	GraphicObject();
	GraphicObject(Point pos, char* fileName);
	~GraphicObject();
	void init();
	void update(float dt);
	void render();
	void mouseOver(int x, int y){};
	void mousePressed(int x, int y){};
	void mouseReleased(int x, int y){};

	void setScreen(SDL_Surface* screen);
	
	Point getPosition();
	void setPosition(Point p);
	
	Point getSize();
	void setSize(Point p);
	
	void setImage(char* filename);
	string getImageFilename();
	
	void setVisible(bool visible);
	bool getVisible();
	
	void dirty();
	bool getDirty();
	void undirty();

protected:
	SDL_Surface *mDrawingScreen;
	string mImageFileName;
	SDL_Surface *mImage;
	Point mPosition;
	Point mSize;
	bool mInitialized;
	bool mVisible;
	bool mDirty;

};


#endif