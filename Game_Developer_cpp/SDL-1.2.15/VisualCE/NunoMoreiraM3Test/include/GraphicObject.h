
#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H

#include "Point.h"
#include "SDL.h"
#include "SDL_image.h"


#include <string>

using namespace std;
//Basic class for renderable objects
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


	//Setters and getters
	void setScreen(SDL_Surface* screen);
	
	Point getPosition();
	void setPosition(Point p);
	
	Point getSize();
	void setSize(Point p);
	
	void setImage(char* filename);
	string getImageFilename();
	
	void setVisible(bool visible);
	bool getVisible();
	

	//Initially planned for drawing only portions of the screen where changes were made. 
	void dirty();
	bool getDirty();
	void undirty();

protected:
	//Surface to draw on
	SDL_Surface *mDrawingScreen;

	//filename of the resource. This should actually be stored on a resourceManager, asking for an asset by a tag and having it return the object
	string mImageFileName;
	
	//The image to draw
	SDL_Surface *mImage;

	//Position of the object
	Point mPosition;

	//Width and Height
	Point mSize;

	//flag for initialization
	bool mInitialized;

	//Visible flag
	bool mVisible;

	//Flag to signal when object needed to be redrawn
	bool mDirty;

};


#endif