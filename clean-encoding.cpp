// clean-encoding.cpp

#include <string>

std::string base64_remove_newlines(std::string encoded_string) {

    std::string decoded_string;

    size_t length = encoded_string.length();

    for (int i = 0; i < length; i++) {
        if (encoded_string[i] == '\n') {
            continue;
        } else {
            decoded_string += encoded_string[i];
        }
    }
    return decoded_string;
}

