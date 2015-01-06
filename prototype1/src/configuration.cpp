#include <configuration.h>

#include <iostream>
#include <istream>
#include <memory>

#include <defaultlinedetector.h>
#include <defaultreconstructor.h>
#include <linedetector.h>
#include <reconstructor.h>

using std::cin;
using std::istream;
using std::make_shared;
using std::shared_ptr;


bool Configuration::debugCloud = true;
bool Configuration::debugLine = true;
shared_ptr<LineDetector> Configuration::lineDetection{make_shared<DefaultLineDetector>()};
shared_ptr<Reconstructor> Configuration::reconstructor{make_shared<DefaultReconstructor>()};
shared_ptr<istream> Configuration::inputStream{&cin, [](istream* p){}};

void Configuration::init(int argc, char *argv[])
{
	for (int i = 0; i < argc; ++i) {
		// TODO
	}
}
