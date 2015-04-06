#ifndef POINTIO_H
#define POINTIO_H

#include <iostream>
#include <istream>
#include <ostream>
#include <regex>
#include <string>

#include "point.h"
#include "vec.h"


Point readPoint();
bool readPoint(Point& p);

void writePoint(const Point& p);

std::ostream& operator<< (std::ostream& stream, const Point& val);
std::istream& operator>> (std::istream& stream, Point& val);


inline Point readPoint()
{
	Point p;
	readPoint(p);
	return p;
}

inline bool readPoint(Point& p)
{
	while (std::cin.good())
	{
		std::string line;
		std::getline(std::cin, line);

		static const std::regex r{"\\s*-?(\\d+|\\d*\\.\\d*)\\s+-?(\\d+|\\d*\\.\\d*)\\s+-?(\\d+|\\d*\\.\\d*)\\s+-?(\\d+|\\d*\\.\\d*)\\s+-?(\\d+|\\d*\\.\\d*)\\s+-?(\\d+|\\d*\\.\\d*)\\s*"};
		// TODO add "\\s+\\d+\\s+\\d+" to regex
		if (std::regex_match(line, r)) {
			std::stringstream{line} >> p;
			return true;
		}
	}
	return false;
}

inline void writePoint(const Point& p)
{
	std::cout << p << "\n";
}

inline std::ostream& operator<< (std::ostream& stream, const Point& val)
{
	stream << val.pos[0] << " " << val.pos[1] << " " << val.pos[2] << " "
		   << val.color[0] << " " << val.color[1] << " " << val.color[2];
// TODO		   << val.pixel[0] << " " << val.pixel[1];
}

inline std::istream& operator>> (std::istream& stream, Point& val)
{
	Vec3d vec3d[2];
	Vec2u vec2u;

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 3; ++j) {
			stream >> vec3d[i][j];
		}
	}
	// TODO uncomment
	//for (int j = 0; j < 2; ++j) {
	//	stream >> vec2u[j];
	//}

	val.pos = vec3d[0];
	val.color = vec3d[1];
	val.pixel = vec2u;
}

#endif // POINTIO_H
