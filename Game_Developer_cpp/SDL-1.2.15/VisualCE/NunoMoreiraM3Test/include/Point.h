
#ifndef POINT_H
#define POINT_H

class Point
{
public:
	Point();
	Point(int x, int y);
	~Point();
	int getX();
	int getY();
	void setX(int nX);
	void setY(int nY);

private:
	int mX;
	int mY;
};

#endif