
#ifndef GEMANIMATOR_H
#define GEMANIMATOR_H

#include "Gem.h"
#include "Point.h"

class GemAnimator
{

public:
	GemAnimator();
	~GemAnimator();
	void moveTo(Gem* g, Point dest, float timeInterval);
	void update(float dt);
	
	bool getFinished();

private:
	float mTimePassed;
	Point mInitialPosition;
	Point mDistance;
	float mTimeInterval;
	bool mFinished;
	Gem* mGem;
};

#endif
