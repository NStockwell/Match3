
#ifndef GEMANIMATOR_H
#define GEMANIMATOR_H

#include "GraphicObject.h"
#include "Point.h"

class GemAnimator
{

public:
	GemAnimator();
	~GemAnimator();
	void update(float dt);

	/**
	* Moves a gem to a x,y coordinate on screen in the time interval
	* @param GraphicObject* g The gem to move
	* @param Point dest The x,y coordinate to move the gem to
	* @param float timeInterval The time for the animation
	*/
	void moveTo(GraphicObject* g, Point dest, float timeInterval);
	
	/**
	* returns true if the timeInterval has passed
	*/
	bool getFinished();

	/**
	* Returns the GraphicObject it is animating
	*/
	GraphicObject* getGem();


private:
	// The time passed on the animation
	float mTimePassed;

	// Initial position of the gem, used to calculate new positions
	Point mInitialPosition;

	// Distance between initial and final position
	Point mDistance;

	// Time the animation will take
	float mTimeInterval;

	// Flag to signal if the animation has finished
	bool mFinished;

	// The gem we are animating
	GraphicObject* mGem;
};

#endif
