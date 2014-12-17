#ifndef POINT_H
#define POINT_H

struct Point
{
	Point(float x, float y, float z, float r, float g, float b) :
		x(x), y(y), z(z),
		r(r), g(g), b(b)
	{
	}
	float x, y, z;
	float r, g, b;
};

#endif // POINT_H
