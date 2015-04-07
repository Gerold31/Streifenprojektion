#ifndef POINTIO_H
#define POINTIO_H

#include <cctype>
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

		const char *s = line.c_str() - 1;
		while (std::isspace(*(++s)));
		if (std::isdigit(*s) || *s == '-' || *s == '.') {
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
	return stream;
}

inline std::istream& operator>> (std::istream& stream, Point& val)
{
	for (int i = 0; i < 3; ++i) {
		stream >> val.pos[i];
	}
	for (int i = 0; i < 3; ++i) {
		stream >> val.color[i];
	}
	// TODO uncomment
	//for (int i = 0; i < 2; ++i) {
	//	stream >> val.pixel[i];
	//}

	return stream;
}

#endif // POINTIO_H
