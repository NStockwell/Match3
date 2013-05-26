#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>


#include "SDL.h"
#include "SDL_thread.h"

#include "GameManager.h"







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


    /* The mouse isn't much use unless we have a display for reference */
    if ( (screen = SDL_SetVideoMode(800, 600, 8, 0)) == NULL ) {
        fprintf(stderr, "Couldn't set 800*600*8 video mode: %s\n",
                        SDL_GetError());
        exit(1);
    }


	GameManager* gm = new GameManager(screen);
	


	

	unsigned int dt = SDL_GetTicks();
	unsigned int currentTime = dt;
	
	
	while(1)
	{
		dt = SDL_GetTicks();
		gm->update((dt - currentTime)  *0.001);
		gm->render();
		
		currentTime = dt;
    }
    /* This should never happen */
    printf("SDL_WaitEvent error: %s\n", SDL_GetError());
    exit(1);
}
