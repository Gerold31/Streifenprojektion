#include <cstdint>
#include <iostream>
#include <regex>
#include <set>

#include <point.h>
#include <pointio.h>

using std::cerr;
using std::endl;
using std::multiset;
using std::regex;
using std::regex_match;
using std::size_t;


struct closer {
	bool operator() (const Point& x, const Point& y) const {
		return x.pos[2] > y.pos[2];
	}
	typedef Point first_argument_type;
	typedef Point second_argument_type;
	typedef bool result_type;
};


static multiset<Point,closer> points;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "Invalid amount of arguments." << endl;
		cerr << endl;
		cerr << "Usage: " << argv[0] << " <amount>" << endl;
		cerr << endl;
		cerr << "amount:" << endl;
		cerr << "    <x>%  Cut x% from the nearest and farthest points" << endl;
		cerr << "    <x>   with x >= 1: Cut x from the nearest and farthest points" << endl;
		cerr << "          with x < 1:  Cut 100*x% from the nearest and farthest points" << endl;
		cerr << endl;
		return EXIT_FAILURE;
	}

	static regex argRegex{"(\\d+(\\.\\d*)?|\\.\\d+)%?"};
	if (!regex_match(argv[1], argRegex)) {
		cerr << "Invalid format to specify amount." << endl;
		return EXIT_FAILURE;
	}

	Point p;
	while (readPoint(p)) {
		points.insert(p);
	}

	double argAmount = atof(argv[1]);
	size_t trunAmount;
	if (argv[1][strlen(argv[1]) - 1] == '%')
		trunAmount = static_cast<size_t>(argAmount * points.size() / 100.);
	else if (argAmount < 1)
		trunAmount = static_cast<size_t>(argAmount * points.size());
	else
		trunAmount = static_cast<size_t>(argAmount);

	int i = 0;
	auto it = points.begin();

	for (; i < trunAmount; ++i)
		it++;
	for (; i < points.size() - trunAmount; ++i)
		writePoint(*(it++));

	return EXIT_SUCCESS;
}
