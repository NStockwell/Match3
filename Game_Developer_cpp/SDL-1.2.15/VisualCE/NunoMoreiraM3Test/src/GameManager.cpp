
#include "GameManager.h"

GameManager::GameManager(SDL_Surface* screen)
{
	mDrawingScreen = screen;
	mState = PreGame;

	mTimePlayed = 0.0f;

	mBackground = new GraphicObject(Point(0,0),"assets\\art\\board\\BackGround.jpg");
	mBackground->setScreen(screen);
	mBackground->init();
	
	mIntro = new GraphicObject(Point(0,0), "assets\\art\\Intro.jpg");
	mIntro->setScreen(screen);
	mIntro->init();
	mIntro->setVisible(true);

	mOutro = new GraphicObject(Point(0,0), "assets\\art\\Outro.jpg");
	mOutro->setScreen(screen);
	mOutro->init();
		
	GemManager::getInstance().addGem(0,"assets/art/Gems/green.png");
	GemManager::getInstance().addGem(1,"assets/art/Gems/blue.png");
	GemManager::getInstance().addGem(2,"assets/art/Gems/red.png");
	GemManager::getInstance().addGem(3,"assets/art/Gems/yellow.png");
	GemManager::getInstance().addGem(4,"assets/art/Gems/purple.png");

	dest.x = 0;
	dest.y = 0;
	dest.w = mBackground->getSize().getX();//w;
	dest.h = mBackground->getSize().getY();//h;


	mBoard = new Board(8, 8, 5, Point(35,35));
	mBoard->setPosition(Point(360,140));
	mBoard->setScreen(screen);

}

GameManager::~GameManager()
{
	delete mBoard;
	delete mBackground;
	delete mIntro;
	delete mOutro;
}

void GameManager::start()
{
	unsigned int dt = SDL_GetTicks();
	unsigned int currentTime = dt;
	
	while(1)
	{
		dt = SDL_GetTicks();
		update((dt - currentTime)  *0.001);
		render();
		
		currentTime = dt;
    }
}

void GameManager::render()
{
	switch(mState)
	{
		case PreGame:
		{
			mIntro->render();
			break;
		}
		case InGame:
		{
			mBackground->render();
			mBoard->render();
			break;
		}
		case PostGame:
		{
			mOutro->render();
			break;
		}
		default:
		{
			mBackground->render();
			break;
		}
	}
	
	SDL_Flip(mDrawingScreen);
}

void GameManager::update(float dt)
{
	SDL_Event ev;
	//Poll events and handle them
	while(SDL_PollEvent(&ev) > 0)
	{
		eventHandler(&ev);
	}
	switch(mState)
	{//update the board only during the InGame state
		case InGame:
		{
			mTimePlayed += dt;
			if(mTimePlayed >= TIME_TO_PLAY)
			{
				mOutro->setVisible(true);
				mBackground->setVisible(false);
				mState = PostGame;
				break;
			}
			mBoard->update(dt);
			break;
		}
	}
	//free cpu cycle
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
					mIntro->setVisible(false);
					mBackground->setVisible(true);
					mState = InGame;						
					mBoard->init();
					break;
				}
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
            exit(0);
        }
        break;
	}
}