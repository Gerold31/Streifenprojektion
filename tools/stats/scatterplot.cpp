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

static double forAxis(char axis, Point point);


static const char latexCode[] = SRC(
			\\begin{tikzpicture}[only marks]\n
			\t	\\begin{axis}[\n
			\t	\t	width=\\linewidth,\n
			\t	\t	line width=1pt,\n
			\t	\t	xlabel={Entfernung},\n
			\t	\t	ylabel={Häufigkeit},\n
			\t	]\n
			\t	\t	\\addplot+[blue] table [x=x, y=y]\n
			\t	\t	\t	{%s};\n
			\t	\\end{axis}\n
			\\end{tikzpicture}\n
);

void scatterplot(string filename, char axisH, char axisV)
{
	ofstream csv{filename};
	csv << "x y" << "\n";
	for (Point p : points) {
		csv << forAxis(axisH, p) << " " << forAxis(axisV, p) << "\n";
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

double forAxis(char axis, Point point)
{
	switch (axis) {
	case 'x':
		return point.pos[0];
	case 'y':
		return point.pos[1];
	case 'z':
		return point.pos[2];
	case 'h':
		return -point.pos[2];
	default:
		cerr << "Invalide axis: " << axis << endl;
		exit(EXIT_FAILURE);
	}
}
