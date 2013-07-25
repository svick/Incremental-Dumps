#pragma once

#include <string>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim);

// makes sure the whole string is converted to an int
long tryParseLong(const std::string &s, bool &success, int radix = 10);