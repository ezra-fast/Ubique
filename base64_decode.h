// base64_decode.h
#pragma once

#include <string>

int find_base64_index(char character);
std::string base64_decode(std::string & encoded_string);
