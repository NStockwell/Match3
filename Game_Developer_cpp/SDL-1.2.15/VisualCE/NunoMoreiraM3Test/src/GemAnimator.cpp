
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
	
	GraphicObject* GemAnimator::getGem()
	{
		return mGem;
	}

	
	void GemAnimator::moveTo(GraphicObject* g, Point dest, float timeInterval)
	{
		//calculate the distance so we can interpolate the position as time goes by
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


		//avoid passing the time interval
		if(mTimePassed >= mTimeInterval){
			mTimePassed = mTimeInterval;
			dt = mTimeInterval - mTimePassed;
			mFinished = true;
			mGem->setVisible(true);
		}
		
		//set the new position according to the time passed in the animation
		if(mGem != NULL)
			mGem->setPosition( Point(mInitialPosition.getX() + (mTimePassed/mTimeInterval) *mDistance.getX(),
								mInitialPosition.getY() + (mTimePassed/mTimeInterval) *mDistance.getY()));
		else
			mFinished = true;
	}
