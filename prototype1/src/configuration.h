#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <istream>
#include <memory>

#include <linedetector.h>
#include <reconstructor.h>


class Configuration
{
public:
	static void init(int argc, char *argv[]);

	static bool debugCloud;
	static bool debugLine;
	static std::shared_ptr<LineDetector> lineDetection;
	static std::shared_ptr<Reconstructor> reconstructor;
	static std::shared_ptr<std::istream> inputStream;
private:
	Configuration() = delete;
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;
};

#endif // CONFIGURATION_H
