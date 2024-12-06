// clean-encoding.h         --> cleaning newlines from base64 strings

#pragma once

#include <string>

std::string base64_remove_newlines(std::string encoded_string);

