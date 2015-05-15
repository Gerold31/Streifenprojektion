#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <utility>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <vec.h>


class Arguments
{
public:
	Arguments(int argc, char *argv[]);

	int getNextOption();
	bool getNextArgument(std::string& param);
	void registerOption(const std::string& opt, int key);

	std::string getOptionString() const;
	const std::vector<std::pair<std::string,int>>& getPossibleOptions() const;

	template<typename type_t>
	bool getNextArgument(type_t& param);

private:
	int argc;
	char **argv;
	std::size_t index = 1, cChar = 0;
	std::string currentOption;
	std::vector<std::pair<std::string,int>> possibleOptions;
	std::map<std::string,int> optionMap;
	std::queue<std::string> params;
	bool strictRefuse = false;
	bool noOptions = false;
};


inline void Arguments::registerOption(const std::string &opt, int key)
{
	optionMap[opt] = key;
}

inline std::string Arguments::getOptionString() const
{
	return currentOption;
}

inline const std::vector<std::pair<std::string, int> > &Arguments::getPossibleOptions() const
{
	return possibleOptions;
}

template<typename type_t>
inline bool Arguments::getNextArgument(type_t& param)
{
	// TODO throw exception for invalid format
	std::string str;
	if (!getNextArgument(str))
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
