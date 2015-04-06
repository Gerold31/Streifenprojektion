#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <libgen.h>
#include <unistd.h>

using std::cerr;
using std::endl;
using std::getenv;
using std::strerror;
using std::string;


int main(int argc, char *argv[])
{
	const char* path = getenv("PATH");
	if (path == nullptr) {
		cerr << "There is no environment variable 'PATH'." << endl;
		return EXIT_FAILURE;
	}

	const char* shell = getenv("SHELL");
	if (shell == nullptr) {
		cerr << "There is no environment variable 'SHELL'." << endl;
		return EXIT_FAILURE;
	}

	const char* cwd = getcwd(nullptr, 0);
	const char* dir = realpath((string{cwd} + "/" +  dirname(argv[0])).c_str(),
			nullptr);

	if (cwd == nullptr || dir == nullptr) {
		cerr << "Could not get directory of the application." << endl;
		cerr << strerror(errno) << endl;
		return EXIT_FAILURE;
	}

	size_t dirlen = strlen(dir);
	char* npath = new char[dirlen + strlen(path) + 2];
	strcpy(&npath[0], dir);
	npath[dirlen] = ':';
	strcpy(&npath[dirlen + 1], path);

	if (setenv("PATH", npath, true) != 0) {
		cerr << "Could not set PATH." << endl;
		cerr << strerror(errno) << endl;
		return EXIT_FAILURE;
	}

	execl(shell, shell, nullptr);

	cerr << "Could not execute the shell." << endl;
	cerr << strerror(errno) << endl;
	return EXIT_FAILURE;
}
