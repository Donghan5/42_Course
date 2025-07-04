#ifndef UTILS_HPP
#define UTILS_HPP

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class Utils {
	public:
		static std::string intToString(int num);
		static std::string floatToString(float num);
		static void cleanUpDoublePointer(char **dptr);
		static std::vector<std::string> split(std::string string, char delim, bool use_whitespaces_delim);
};

#endif
