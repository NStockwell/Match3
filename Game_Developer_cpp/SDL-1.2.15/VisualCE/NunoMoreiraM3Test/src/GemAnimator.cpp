
#include "GemAnimator.h"

	GemAnimator::GemAnimator()
	{
		mTimePassed =0;
		mDistance = Point(0,0);
		mTimeInterval = 0;
	}
	GemAnimator::~GemAnimator()
	{}

	bool GemAnimator::getFinished()
	{
		return mFinished;
	}

	void GemAnimator::moveTo(Gem* g, Point dest, float timeInterval)
	{
		mGem = g;
		mInitialPosition = mGem->getPosition();
		mDistance = Point(dest.getX() - g->getPosition().getX(),dest.getY() - g->getPosition().getY()) ;
		mTimeInterval = timeInterval;
		mFinished = false;
	}

	void GemAnimator::update(float dt)
	{
		if(mFinished)
			return;

		mTimePassed += dt;

		if(mTimePassed >= mTimeInterval){
			mTimePassed = mTimeInterval;
			dt = mTimeInterval - mTimePassed;
			mFinished = true;
		}
		
		if(mGem != NULL)
			mGem->setPosition(mInitialPosition.getX() + (mTimePassed/mTimeInterval) *mDistance.getX(),
								mInitialPosition.getY() + (mTimePassed/mTimeInterval) *mDistance.getY());
		else
			mFinished = true;
	}
