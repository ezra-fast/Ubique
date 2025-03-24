// split.cpp defines the split() function, which takes a string and returns a vector of substrings, split along the given delimeter

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

/*
 * return tokens: this function returns a vector of strings, those being the substrings of the C2 command that were separated by ':'
 *      - in most cases, this will be an instruction (vector.at(0)) and a file/module (vector.at(1))
 *
 * algorithm:
 *      for each letter in whole_string
 *          if the letter is not the delimeter and the letter is not " "
 *              add the letter to the current word bucket
 *          else if the letter is the delimeter
 *              append the contents of the current word bucket to the vector
 *              clear the current word bucket
 *
 * */

std::vector<std::string> split(std::string whole_string, char delimeter) {

    std::vector<std::string> tokens;
    std::stringstream word_bucket;

    for (int i = 0; i < whole_string.length(); i++) {

        if (whole_string[i] != delimeter && (char)whole_string[i] != ' ') {
            word_bucket << whole_string[i];
        } else if (whole_string[i] == delimeter) {
            tokens.push_back(word_bucket.str());
            word_bucket.str("");                        // this is resetting the contents of the stream
            word_bucket.clear();                        // this is clearing any remaining flags; this is just good practice
        }
        if (i == whole_string.length() - 1) {
            tokens.push_back(word_bucket.str());        // no need to clear the buffer because this block is only executed for the last string
        }
    }

    return tokens;

}

