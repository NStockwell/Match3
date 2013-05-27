
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H


//#include "SoundManager.h"
#include "Board.h"
#include "Point.h"
#include "GemManager.h"

#include "SDL.h"

#define TIME_TO_PLAY 60

class GameManager
{
public:
	GameManager(SDL_Surface* screen);
	~GameManager();
	void render();
	void update(float dt);
	void start();

private:
	//Possible states for the game
	enum State{ PreGame, InGame, PausedGame, PostGame };
	State mState;

	//Board containing the gems
	Board* mBoard;

	//Timer for end of demo
	float mTimePlayed;
	
	//
	SDL_Rect dest;

	//Mine entrance background
	GraphicObject *mBackground;
	//Intro image
	GraphicObject *mIntro;
	//Outro Image
	GraphicObject *mOutro;

	//Game Window
	SDL_Surface *mDrawingScreen;

	//Handler for events
	int eventHandler(SDL_Event *ev);
};

#endif