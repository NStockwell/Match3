
#include "Point.h"


Point::Point(float x, float y)
{
	mX = x;
	mY = y;
}

Point::Point()
{
	mX = mY = 0;
}
Point::~Point()
{}


void Point::setX(float nX)
{
	mX = nX;
}

void Point::setY(float nY)
{
	mY = nY;
}

float Point::getX() const
{
	return mX;
}

float Point::getY() const
{
	return mY;
}

float Point::getLength()
{
	return sqrt(mX*mX + mY*mY);
}