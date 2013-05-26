
#include "GameManager.h"

GameManager::GameManager(SDL_Surface* screen)
{
	mDrawingScreen = screen;
	mState = PreGame;
	mBoard = new Board(8,8,5,Point(35,35));

	mTimePlayed = 0.0f;

	mBackground = IMG_Load("assets\\art\\board\\BackGround.jpg");
	if(mBackground == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", "assets\\art\\board\\BackGround.jpg", SDL_GetError());
		return;
	}

		
	GemManager::getInstance().addGem(0,"assets/art/Gems/green.png");
	GemManager::getInstance().addGem(1,"assets/art/Gems/blue.png");
	GemManager::getInstance().addGem(2,"assets/art/Gems/red.png");
	GemManager::getInstance().addGem(3,"assets/art/Gems/yellow.png");
	GemManager::getInstance().addGem(4,"assets/art/Gems/purple.png");

	dest.x = 0;
	dest.y = 0;
	dest.w = mBackground->w;
	dest.h = mBackground->h;


	mBoard = new Board(8,8,/*"assets\\art\\board\\BackGround.jpg",*/5,Point(35,35));
	mBoard->setPosition(Point(360,140));
	mBoard->setScreen(screen);

}

GameManager::~GameManager()
{
	delete mBoard;
}

void GameManager::render()
{
	switch(mState)
	{
		case InGame:
		{
			if(mBoard->isAnimating())
			{
				SDL_BlitSurface(mBackground, NULL, mDrawingScreen, &dest);
				SDL_UpdateRects(mDrawingScreen, 1, &dest);
			}
			mBoard->render();
			break;
		}
		default:
		{
			SDL_BlitSurface(mBackground, NULL, mDrawingScreen, &dest);
			SDL_UpdateRects(mDrawingScreen, 1, &dest);
			break;
		}
	}
}

void GameManager::update(float dt)
{
	SDL_Event ev;
	while(SDL_PollEvent(&ev) > 0)
	{
		eventHandler(&ev);
	}
	switch(mState)
	{
	case InGame:
		{
			mTimePlayed += dt;
			if(mTimePlayed >= TIME_TO_PLAY)
			{
				mState = PostGame;
				break;
			}
			mBoard->update(dt);
			break;
		}
	}
	
	SDL_Delay(1);
}

int GameManager::eventHandler(SDL_Event *ev)
{
	switch (ev->type) 
	{
        case SDL_ACTIVEEVENT: {
            if ( ev->active.state & SDL_APPACTIVE ) {
                if ( ev->active.gain ) {
					if(mState == PausedGame)
						mState = InGame;
                } else {
					if(mState == InGame)
						mState = PausedGame;
                }
            }
			break;
        }
                    
        case SDL_MOUSEBUTTONDOWN: {
            Uint8 *keys;

            keys = SDL_GetKeyState(NULL);
            if ( keys[SDLK_ESCAPE] == SDL_PRESSED ) {
                printf("Bye bye...\n");
                exit(0);
            }
            printf("Mouse button pressed\n");
			switch(mState)
			{
				case InGame:
				{
					mBoard->mousePressed(ev->motion.x, ev->motion.y);
					break;
				}
				case PreGame:
				{
					mState = InGame;						
					mBoard->init();
				}break;
			}
			break;
        }
        
		case SDL_MOUSEBUTTONUP:{
			switch(mState)
			{
				case InGame:
				{
					mBoard->mouseReleased(ev->motion.x, ev->motion.y);
					break;
				}
			}
			break;	
		}
		case SDL_MOUSEMOTION : {
			printf("Mouse moved to (%d,%d)\n",
					ev->motion.x, ev->motion.y);
				
			switch(mState)
			{
				case InGame:
				{
					mBoard->mouseOver(ev->motion.x, ev->motion.y);
					break;
				}
			}
			break;
		}
        case SDL_QUIT: {
            printf("Quit requested, quitting.\n");

			//delete mBoard;
            exit(0);
        }
        break;
	}
}