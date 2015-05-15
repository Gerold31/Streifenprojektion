#include "arguments.h"

#include <climits>
#include <string>

using std::string;

static bool isOption(char *arg);
static string strPlus1(const string& str);


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
					// It is a '--' -> there are no options until here
					++index;
					noOptions = true;
					return 0;
				} else {
					// It is a long option (--abc)
					char *opt = argv[index];
					// cut if there is an '='
					// TODO error if there is an '=' but the option does not support it
					while (opt[cChar] != '=' && opt[cChar] != '\0')
						++cChar;
					if (opt[cChar] == '=')
						opt[cChar++] = '\0';
					// check if there is an option with this name
					currentOption = opt;
					auto it = optionMap.find(currentOption);
					if (it != optionMap.end()) {
						return it->second;
					}
					// check if the option can be extended
					possibleOptions.assign(optionMap.lower_bound(currentOption),
								optionMap.lower_bound(strPlus1(currentOption)));
					switch (possibleOptions.size()) {
					case 0:
						return -1;
						break;
					case 1:
						currentOption = possibleOptions[0].first;
						return possibleOptions[0].second;
						break;
					default:
						return -2;
						break;
					}
				}
			} else {
				// Its is a normal argument with one or more options
				cChar = 1;
			}
		} else {
			// There is no argument anymore
			noOptions = true;
			return 0;
		}
	}

	currentOption = {'-', argv[index][cChar], '\0'};
	auto it = optionMap.find(currentOption);
	++cChar;
	if (it == optionMap.end()) {
		return strictRefuse ? -1 : currentOption[1];
	} else {
		return it->second;
	}
}

bool Arguments::getNextArgument(string& param)
{
	if (noOptions && !params.empty()) {
		param = params.front();
		params.pop();
		return true;
	}

	if (cChar != 0 && argv[index][cChar] == '\0'
			&& argv[index][cChar - 1] != '\0') {
		++index;
		cChar = 0;
	}

	if (index < argc) {
		param = &argv[index++][cChar];
		cChar = 0;
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

string strPlus1(const string& str)
{
	string tmp = str;
	for (int i = tmp.size() - 1; i >= 0; --i) {
		if (tmp[i] < CHAR_MAX) {
			++tmp[i];
			break;
		}
	}
	return tmp;
}
