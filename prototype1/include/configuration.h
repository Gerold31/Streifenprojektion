#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <istream>
#include <memory>

#include "controller.h"
#include "lightbardetector.h"
#include "reconstructor.h"


class Configuration
{
public:
	static void init(int argc, char *argv[]);

	static bool createCommands;
	static bool debugLightbar;
	static bool debugCamera;
	static bool debugHeightmap;
	static bool verbose;
	static bool verbose2;
	static bool waitKey;
	static std::shared_ptr<Controller> controller;
	static std::shared_ptr<LightBarDetector> lightBarDetector;
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
