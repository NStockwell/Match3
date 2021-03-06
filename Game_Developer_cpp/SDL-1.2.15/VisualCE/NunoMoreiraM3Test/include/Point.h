
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
	//getters and setters
	float getX() const;
	float getY() const;
	void setX(float nX);
	void setY(float nY);

	//returns the length of the Point
	float getLength();

	//overloading operators
	inline bool operator==(const Point &p) { return ((p.getX() == getX()) && (p.getY() == getY()));}
	inline bool operator!=(const Point &p) {return !(*this==p);}
	inline bool operator<(const Point &p) {return ( getX() < p.getX()) || ((getX() == p.getX()) && (getY() < p.getY())); }
	inline bool operator>(const Point &p) { return !(*this < p);}
	inline Point operator-(const Point &p) {Point result; result.setX(getX() - p.getX()); result.setY(getY() - p.getY()); return result; }
	inline Point operator+(const Point &p) {Point result; result.setX(getX() + p.getX()); result.setY(getY() + p.getY()); return result; }

private:

	float mX;
	float mY;
};

#endif