
#ifndef POINT_H
#define POINT_H

#define NULL_POINT Point(-1,-1)

#include <math.h>

class Point
{
public:
	Point();
	Point(float x, float y);
	~Point();
	float getX() const;
	float getY() const;
	void setX(float nX);
	void setY(float nY);
	float getLength();
	inline bool operator==(const Point &p) { return ((p.getX() == getX()) && (p.getY() == getY()));}

	
	inline bool operator!=(const Point &p) {return !(*this==p);}

	inline Point operator-(const Point &p) {Point result; result.setX(getX() - p.getX()); result.setY(getY() - p.getY()); return result; }
	inline Point operator+(const Point &p) {Point result; result.setX(getX() + p.getX()); result.setY(getY() + p.getY()); return result; }

private:
	float mX;
	float mY;
};

#endif