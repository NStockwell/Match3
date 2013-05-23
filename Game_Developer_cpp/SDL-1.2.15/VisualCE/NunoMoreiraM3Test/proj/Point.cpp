
#include "Point.h"


Point::Point(int x, int y)
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

void Point::setX(int nX)
{
	mX = nX;
}

void Point::setY(int nY)
{
	mY = nY;
}

int Point::getX()
{
	return mX;
}

int Point::getY()
{
	return mY;
}