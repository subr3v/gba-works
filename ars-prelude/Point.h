/********************************************************************
	created:	2014/03/12
	created:	12:3:2014   17:45
	filename: 	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair\Point.h
	file path:	C:\Users\Stefano\Desktop\uni-repo\gba-semester2\DungeonOfDespair
	file base:	Point
	file ext:	h
	author:		Stefano Musumeci

	purpose:	Very basic point class
*********************************************************************/
#ifndef Point_h__
#define Point_h__

#include <stdint.h>

template <typename T>
struct Point
{
    T x;
    T y;
    Point() { x = y = 0; }
    Point(T x, T y) { this->x = x; this->y = y; }
};

typedef Point<short> Point16;

#endif // Point_h__
