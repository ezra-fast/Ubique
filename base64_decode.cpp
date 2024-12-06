#include <string>
#include <vector>
#include <iostream>
#include "base64_decode.h"

int find_base64_index(char character) {
    if (character >= 'A' && character <= 'Z')
        return character - 'A';
    if (character >= 'a' && character <= 'z')
        return character - 'a' + 26;
    if (character >= '0' && character <= '9')
        return character - '0' + 52;
    if (character == '+') 
        return 62;
    if (character == '/')
        return 63;
    return 1;
}

std::string base64_decode(std::string & encoded_string) {
    std::string decoded_string;
    int padding = 0;

    if (encoded_string.empty() == false && encoded_string.back() == '=') {              // this needs to calculate the padding of the base64 string
        padding++;

        auto encoded_string_length = encoded_string.length();

        if (encoded_string[encoded_string_length - 2] == '=') {
            padding++;
        }
    }

    size_t decoded_string_length = (encoded_string.length() * 3) / 4 - padding;
    decoded_string.resize(decoded_string_length);

    size_t decoded_index = 0;
    size_t encoded_index = 0;

    while (encoded_index < encoded_string.length()) {
        uint32_t buffer = 0;
        int bits_filled = 0;

        for (int i = 0; i < 4 && encoded_index < encoded_string.length(); ++i) {
            char tmp = encoded_string[encoded_index++];
            if (tmp == '=') {break;} else {
                buffer = (buffer << 6) | find_base64_index(tmp);
                bits_filled += 6;
            }

        }

        while (bits_filled >= 8) {
            bits_filled -= 8;
            decoded_string[decoded_index++] = static_cast<char>((buffer >> bits_filled) & 0xFF);
        }
    }

    return decoded_string;
}

//int main() {

    //std::string the_string = "dGhpcyBpcyBhIHRlc3QK\n";

    //std::cout << base64_decode(the_string) << '\n';

    //return 0;
//}
