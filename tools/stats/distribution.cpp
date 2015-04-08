#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctgmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <point.h>

#include "common.h"

using std::cerr;
using std::endl;
using std::exit;
using std::fmod;
using std::free;
using std::ofstream;
using std::pair;
using std::sprintf;
using std::string;
using std::vector;


static const char latexCode[] = SRC(
			\\begin{tikzpicture}\n
			\t	\\begin{axis}[\n
			\t	\t	width=\\linewidth,\n
			\t	\t	line width=1pt,\n
			\t	\t	xlabel={Entfernung},\n
			\t	\t	ylabel={Häufigkeit},\n
			\t	]\n
			\t	\t	\\addplot+[blue] table [x=dist, y=count]\n
			\t	\t	\t	{%s};\n
			\t	\\end{axis}\n
			\\end{tikzpicture}\n
);

void distribution(string filename)
{
	vector<pair<double, unsigned int>> appearances;
	double current = (long) (-points.begin()->pos[2] / step) * step + step / 2.;
	for (Point p : points) {
		while ( (-p.pos[2] - current) > (current + step + p.pos[2])) {
			current += step;
			appearances.emplace_back(current, 0);
		}
		if (appearances.empty()) {
			appearances.emplace_back(current, 0);
		}
		appearances.back().second += 1;
	}

	ofstream csv{filename};
	csv << "dist count" << "\n";
	for (pair<double, unsigned int> entry : appearances) {
		csv << entry.first << " " << entry.second << "\n";
	}
	csv.close();

	char *absolute = realpath(filename.c_str(), nullptr);
	if (absolute == nullptr) {
		cerr << "Could not get absolute path to csv file." << endl;
		exit(EXIT_FAILURE);
	}

	static char buf[1024]; // This makes the funktion unsafe for multithreading
	sprintf(buf, latexCode, absolute);
	free(absolute);
	latex << buf;
}
