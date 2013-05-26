#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>


#include "SDL.h"
#include "SDL_thread.h"


#include "Point.h"
#include "Board.h"
#include "GemManager.h"


Board* board;
SDL_Surface *mBackground;




int eventHandler(SDL_Event *ev)
{

	switch (ev->type) 
	{
        case SDL_ACTIVEEVENT: {
            if ( ev->active.state & SDL_APPACTIVE ) {
                if ( ev->active.gain ) {
                    printf("App activated\n");
                } else {
                    printf("App iconified\n");
                }
            }
        }
        break;
                    
        case SDL_MOUSEBUTTONDOWN: {
            Uint8 *keys;

            keys = SDL_GetKeyState(NULL);
            if ( keys[SDLK_ESCAPE] == SDL_PRESSED ) {
                printf("Bye bye...\n");
                exit(0);
            }
            printf("Mouse button pressed\n");
			board->mousePressed(ev->motion.x, ev->motion.y);
        }
        break;


        case SDL_QUIT: {
            printf("Quit requested, quitting.\n");

			delete board;
            exit(0);
        }
        break;
	}
}


/* This function may run in a separate event thread */
int FilterEvents(const SDL_Event *event) {
    static int boycott = 0;

    /* This quit event signals the closing of the window */
    if ( (event->type == SDL_QUIT) && boycott ) {
        printf("Quit event filtered out -- try again.\n");
        boycott = 0;
        return(0);
    }
    if ( event->type == SDL_MOUSEMOTION ) {
        printf("Mouse moved to (%d,%d)\n",
                event->motion.x, event->motion.y);
		if(board != NULL)
			board->mouseOver(event->motion.x, event->motion.y);
        return(0);    /* Drop it, we've handled it */
    }

	   if ( event->type == SDL_MOUSEBUTTONDOWN) {
                Uint8 *keys;

                keys = SDL_GetKeyState(NULL);
                if ( keys[SDLK_ESCAPE] == SDL_PRESSED ) {
                    printf("Bye bye...\n");
                    exit(0);
					

                }
				board->mousePressed(event->motion.x, event->motion.y);
                printf("Mouse button pressed\n");
				return(0);
	   }
    return(1);
}



int main(int argc, char *argv[])
{
	SDL_Surface *screen;

    /* Initialize the SDL library (starts the event loop) */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Clean up on exit, exit on window close and interrupt */
    atexit(SDL_Quit);

    /* Ignore key events */
    SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);

    /* Filter quit and mouse motion events */
    //SDL_SetEventFilter(FilterEvents);

	//SDL_Thread *thread;

 //   thread = SDL_CreateThread(eventHandler, NULL);
 //   if ( thread == NULL ) {
 //       fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
 //       return 0;
 //   }


    /* The mouse isn't much use unless we have a display for reference */
    if ( (screen = SDL_SetVideoMode(800, 600, 8, 0)) == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
                        SDL_GetError());
        exit(1);
    }


	mBackground = IMG_Load("assets\\art\\board\\BackGround.jpg");
	if(mBackground == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", "assets\\art\\board\\BackGround.jpg", SDL_GetError());
		return 0;
	}

		
	GemManager::getInstance().addGem(0,"assets/art/Gems/green.png");
	GemManager::getInstance().addGem(1,"assets/art/Gems/blue.png");
	GemManager::getInstance().addGem(2,"assets/art/Gems/red.png");
	GemManager::getInstance().addGem(3,"assets/art/Gems/yellow.png");
	GemManager::getInstance().addGem(4,"assets/art/Gems/purple.png");


	board = new Board(8,8,/*"assets\\art\\board\\BackGround.jpg",*/5,Point(35,35));
	board->setPosition(330,100);
	board->setScreen(screen);
	board->init();
	

	SDL_Rect dest;

	dest.x = 0;
	dest.y = 0;
	dest.w = mBackground->w;
	dest.h = mBackground->h;

	unsigned int dt = SDL_GetTicks();
	unsigned int currentTime = dt;
	SDL_Event ev;
	
	SDL_BlitSurface(mBackground, NULL, screen, &dest);
	SDL_UpdateRects(screen, 1, &dest);

	while(1)
	{
		
		while(SDL_PollEvent(&ev) > 0)
		{
			eventHandler(&ev);
		}

		if(board)
		{

			dt = SDL_GetTicks();
			//printf("temp (%d)\n", dt - currentTime);
        

			board->update((dt - currentTime)  *0.001);
			if(board->isAnimating())
			{
				SDL_BlitSurface(mBackground, NULL, screen, &dest);
				SDL_UpdateRects(screen, 1, &dest);
			}
			board->render();
			currentTime = dt;
		}

		
    }
    /* This should never happen */
    printf("SDL_WaitEvent error: %s\n", SDL_GetError());
    exit(1);
}
