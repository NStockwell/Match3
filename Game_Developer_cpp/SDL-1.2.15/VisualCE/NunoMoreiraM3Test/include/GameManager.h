
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
	enum State{ PreGame, InGame, PausedGame, PostGame };
	State mState;
	Board* mBoard;
	float mTimePlayed;
	
	SDL_Rect dest;
	SDL_Surface *mBackground;
	SDL_Surface *mIntro;
	SDL_Surface *mOutro;
	SDL_Surface *mDrawingScreen;

	
	int eventHandler(SDL_Event *ev);
};

#endif