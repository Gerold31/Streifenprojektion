#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <arguments.h>
#include <point.h>
#include <pointio.h>

#include "common.h"

using std::cerr;
using std::cout;
using std::endl;
using std::multiset;
using std::ostringstream;
using std::strerror;
using std::string;
using std::tmpnam;


double step = 2;
multiset<Point,closer> points;
ostringstream latex;
char workingDirectory[L_tmpnam] = {'\0'};

static bool pointsRead = false;
static void readPoints();

int main(int argc, char *argv[])
{
	bool showResult = false;
	bool quiet = false;

	// create header for latex
	latex << "\\documentclass[ngerman,a4paper,parskip=half]{scrartcl}" << endl
		  << "\\usepackage{babel}" << endl
		  << "\\usepackage{tikz,pgfplots}" << endl
		  << "\\begin{document}" << endl
		  << endl;

	// check arguments
	Arguments args{argc, argv};
	int opt;
	while (opt = args.getNextOption()) {
		switch (opt) {
		case 'd':
		{
			string filename;
			if (!args.getNextParam(filename)) {
				cerr << "Missing filename for distribution." << endl;
				return EXIT_FAILURE;
			}
			readPoints();
			distribution(filename);
			break;
		}
		case 'h':
		case '?':
			cerr << "Usage: " << argv[0] << "[options and tools]" << endl;
			cerr << endl;
			cerr << "Options:" << endl;
			cerr << "    -h      -?" << endl;
			cerr << "        Display this page." << endl;
			cerr << endl;
			cerr << "    -q" << endl;
			cerr << "        Disable output of LaTeX sources." << endl;
			cerr << endl;
			cerr << "    -s" << endl;
			cerr << "        Compile LaTeX sources and show result." << endl;
			cerr << endl;
			cerr << "    -w <step-size>" << endl;
			cerr << "        Set the size of steps. This option must be specified" << endl;
			cerr << "        before you specify the tool, you want to use." << endl;
			cerr << "        The default step-size is 2." << endl;
			cerr << endl;
			cerr << "Tools:" << endl;
			cerr << "    -d" << endl;
			cerr << "        Create sources to show the distribution with a histogram." << endl;
			cerr << "        This tool uses the setp-size." << endl;
			cerr << endl;
			return EXIT_SUCCESS;
			break;
		case 'q':
			quiet = true;
			break;
		case 's':
			showResult = true;
			break;
		case 'w':
		{
			if (!args.getNextParam(step)) {
				cerr << "Missing step-size for option "
					 << args.getOptionString() << "." << endl;
				return EXIT_FAILURE;
			}
			break;
		}
		default:
			cerr << "Invalid option: " << args.getOptionString() << endl;
			return EXIT_FAILURE;
			break;
		}
	}
	string arg;
	if (args.getNextParam(arg)) {
		cerr << "Too many arguments: " << arg << endl;
		return EXIT_FAILURE;
	}

	// create tail of latex source
	latex << "\\end{document}" << endl;

	// write latex source to stdout if not quiet
	if (!quiet)
		cout << latex.str();

	// compile and show result if requested
	if (showResult) {
		// create working directory
		// TODO tmpnam ist not a very good solution
		// TODO working directory will not be deleted yet
		if (tmpnam(workingDirectory) == nullptr) {
			cerr << "Could not create a working directory for LaTeX." << endl;
			return EXIT_FAILURE;
		}
		if (mkdir(workingDirectory, 0700) < 0) {
			cerr << "Could not create a working directory for LaTeX: "
				 << strerror(errno) << endl;
			return EXIT_FAILURE;
		}
		// create pipe to push sources to latex
		int fildes[2];
		if (pipe(fildes) < 0) {
			cerr << "Could not create pipe to run LaTeX: "
				 << strerror(errno) << endl;
			return EXIT_FAILURE;
		}
		// create process and run latex
		pid_t pid = fork();
		if (pid == 0) {
			// child process
			// redirect to stdin
			if (dup2(fildes[0], 0) < 0) {
				cerr << "Could not redirect sources for LaTeX: "
					 << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			// close unused file discriptors
			if (close(fildes[0]) < 0 || close(fildes[1]) < 0) {
				cerr << "Could not close file discriptor: "
					 << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			// change into working directory
			if (chdir(workingDirectory) < 0) {
				cerr << "Could not change into working directory for LaTeX: "
					 << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			// run
			execlp("pdflatex", "pdflatex", /*"-interaction=batchmode",*/ nullptr);
			// something failed
			cerr << "Could not run LaTeX: " << strerror(errno) << endl;
			exit(EXIT_FAILURE);
		} else if (pid > 0) {
			// parent process
			// close unused file discriptors
			if (close(fildes[0]) < 0) {
				cerr << "Could not close file discriptor: "
					 << strerror(errno) << endl;
				return EXIT_FAILURE;
			}
			// send sources to stdin of child
			string src = latex.str();
			ssize_t count = src.size();
			while (count > 0) {
				ssize_t ret = write(fildes[1], src.c_str(), count);
				if (ret < 0) {
					cerr << "Could not write sources to pipe: "
						 << strerror(errno) << endl;
					return EXIT_FAILURE;
				}
				count -= ret;
			}
			if (close(fildes[1]) < 0) {
				cerr << "Could not close file discriptor: "
					 << strerror(errno) << endl;
				return EXIT_FAILURE;
			}
			// wait for child
			int status;
			waitpid(pid, &status, 0);
			if (status != EXIT_SUCCESS)
				return status;
		} else {
			// fork failed
			cerr << "Could not create process to run LaTeX: "
				 << strerror(errno) << endl;
			return EXIT_FAILURE;
		}
		// create process and show pdf
		pid = fork();
		if (pid == 0) {
			// child process
			// change into working directory
			if (chdir(workingDirectory) < 0) {
				cerr << "Could not change into working directory to view PDF: "
					 << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			// run
			execlp("xdg-open", "xdg-open", "texput.pdf", nullptr);
			// something failed
			cerr << "Could not run PDF viewer: " << strerror(errno) << endl;
			exit(EXIT_FAILURE);
		} else if (pid > 0) {
			// parent process
			// wait for child
			int status;
			waitpid(pid, &status, 0);
			if (status != EXIT_SUCCESS)
				return status;
		} else {
			// fork failed
			cerr << "Could not create process to show pdf: "
				 << strerror(errno) << endl;
			return EXIT_FAILURE;
		}
	}
}

void readPoints()
{
	if (pointsRead)
		return;
	pointsRead = true;

	Point p;
	while (readPoint(p)) {
		points.insert(p);
	}
}
