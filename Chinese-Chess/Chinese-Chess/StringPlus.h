#pragma once

#include <string>
#include <vector>

//using namespace std;

class StringPlus {
public:
	static std::string ltrim(const std::string& inString) {
		auto firstNonSpace = inString.find_first_not_of(" \t");
		if (firstNonSpace == -1)
			return "";
		else
			return inString.substr(firstNonSpace);
	}
	static std::string rtrim(const std::string& inString) {
		return inString.substr(0, inString.find_last_not_of(" \t") + 1);
	}
	static std::string trim(const std::string& inString) {
		return ltrim(rtrim(inString));
	}
	//static std::vector<std::string> split(std::string str, std::string delim) {
	//	char* cstr = const_cast<char*>(str.c_str());
	//	char* current;
	//	std::vector<std::string> arr;
	//	current = strtok(cstr, delim.c_str());
	//	while (current != NULL) {
	//		arr.push_back(current);
	//		current = strtok(NULL, delim.c_str());
	//	}
	//	return arr;
	//}

};