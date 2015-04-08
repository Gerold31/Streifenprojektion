#ifndef COMMON
#define COMMON

#include <cstdio>
#include <set>
#include <sstream>
#include <string>

#include <point.h>


#define SRC(...) #__VA_ARGS__

struct closer {
	bool operator() (const Point& x, const Point& y) const {
		return x.pos[2] > y.pos[2];
	}
	typedef Point first_argument_type;
	typedef Point second_argument_type;
	typedef bool result_type;
};

extern double step;
extern std::multiset<Point,closer> points;
extern std::ostringstream latex;
extern char workingDirectory[L_tmpnam];

void distribution(std::string filename);

#endif // COMMON
