#include "arguments.h"

#include <string>

using std::string;

static bool isOption(char *arg);


Arguments::Arguments(int argc, char *argv[]) :
	argc{argc}, argv{argv}
{
}

int Arguments::getNextOption()
{
	if (noOptions)
		return 0;

	if (cChar != 0 && argv[index][cChar] == '\0') {
		++index;
		cChar = 0;
	}

	if (cChar == 0) {
		for (; index < argc && !isOption(argv[index]); ++index) {
			params.push(argv[index]);
		}
		if (index < argc) {
			if (argv[index][1] == '-') {
				if (argv[index][2] == '\0') {
					// It is a '--' -> everything is a parameter until here
					++index;
					noOptions = true;
					return 0;
				} else {
					// It is a long option
					currentOption = argv[index];
					auto it = optionMap.find(currentOption.substr(1));
					++index;
					if (it == optionMap.end()) {
						return -1;
					} else {
						return it->second;
					}
				}
			} else {
				// Its is a normal argument with one or more short options
				cChar = 1;
			}
		} else {
			// There is no argument anymore
			noOptions = true;
			return 0;
		}
	}

	currentOption = {'-', argv[index][cChar], '\0'};
	auto it = optionMap.find(currentOption.substr(1));
	++cChar;
	if (it == optionMap.end()) {
		return strictRefuse ? -1 : currentOption[1];
	} else {
		return it->second;
	}
}

bool Arguments::getNextParam(string& param)
{
	if (noOptions && !params.empty()) {
		param = params.front();
		params.pop();
		return true;
	}

	if (cChar != 0 && argv[index][cChar] == '\0') {
		++index;
		cChar = 0;
	}

	if (cChar != 0) {
		param = &argv[index++][cChar];
		cChar = 0;
		return true;
	}

	if (index < argc) {
		param = argv[index++];
		return true;
	} else {
		noOptions = true;
		return false;
	}
}


bool isOption(char *arg)
{
	return arg[0] == '-' && arg[1] != '\0';
}
