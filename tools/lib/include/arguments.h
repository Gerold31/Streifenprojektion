#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <queue>
#include <regex>
#include <sstream>
#include <string>

#include <vec.h>


class Arguments
{
public:
	Arguments(int argc, char *argv[]);

	int getNextOption();
	bool getNextParam(std::string& param);

	std::string getOptionString() const;

	template<typename type_t>
	bool getNextParam(type_t& param);

private:
	int argc;
	char **argv;
	std::size_t index = 1, cChar = 0;
	std::string currentOption;
	std::unordered_map<std::string,int> optionMap;
	std::queue<std::string> params;
	bool strictRefuse = false;
	bool noOptions = false;
};


inline std::string Arguments::getOptionString() const
{
	return currentOption;
}

template<typename type_t>
inline bool Arguments::getNextParam(type_t& param)
{
	// TODO throw exception for invalid format
	std::string str;
	if (!getNextParam(str))
		return false;
	std::istringstream stream{str};

	stream >> param;

	if (stream.fail())
		return false; // TODO exception
	while (std::isspace(stream.peek()))
		stream.ignore();
	if (stream.peek() != EOF)
		return false; // TODO exception

	return true;
}

#endif // ARGUMENTS_H
