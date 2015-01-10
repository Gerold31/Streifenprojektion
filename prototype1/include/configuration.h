#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <istream>
#include <memory>

#include <lightbardetector.h>
#include <reconstructor.h>


class Configuration
{
public:
	static void init(int argc, char *argv[]);

	static bool debugCloud;
	static bool debugLine;
	static bool debugCamera;
	static bool verbose;
	static std::shared_ptr<LightBarDetector> lineDetection;
	static std::shared_ptr<Reconstructor> reconstructor;
	static std::shared_ptr<std::istream> inputStream;
	static int captureDevice;
	static DeviceConfiguration deviceConfiguration;
	static const char *savePrefix;

private:
	Configuration() = delete;
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;

	static bool equal(const char *str, const char *op);
	static const char* getParam();
	static void handleOption(const char op);
	static bool helpAndExit(int exitCode  = EXIT_FAILURE);
};

#endif // CONFIGURATION_H
