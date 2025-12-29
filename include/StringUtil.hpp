#pragma once
#include <string>

namespace strutil {
std::string toLower(std::string s);
bool icontains(const std::string& haystack, const std::string& needle);
} // namespace strutil
