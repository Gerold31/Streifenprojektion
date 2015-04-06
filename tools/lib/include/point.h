#ifndef POINT_H
#define POINT_H

#include "vec.h"


class Point final
{
public:
	Point() = default;
	Point(const Vec3d& pos);
	Point(const Vec3d& pos, const Vec3d& color, const Vec2u& pixel);

	Vec3d pos;
	Vec3d color;
	Vec2u pixel;
};


inline Point::Point(const Vec3d& pos) :
	pos{pos}
{
}

inline Point::Point(const Vec3d& pos, const Vec3d& color, const Vec2u& pixel) :
	pos{pos}, color{color}, pixel{pixel}
{
}

#endif // POINT_H
