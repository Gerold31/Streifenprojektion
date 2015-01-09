#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <istream>
#include <memory>


class Configuration
{
public:
	static void init(int argc, char *argv[]);

	static bool verbose;
	static bool drawBox;
	static std::shared_ptr<std::istream> inputStream;
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
