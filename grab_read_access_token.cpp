// reading the GitHub PAT from PATs.txt and returning it as a string

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

std::string grab_read_access_token() {

    std::ifstream file("PATs.txt");
    if (!file.is_open()) {std::cout << "[!] Cannot open PATs.txt to retrieve the token!" << '\n'; return "FAILED_NO_TOKEN";}

    std::string line;

    if (std::getline(file, line)) {
        file.close();
        return line;
    } else {
        return "FAILED_NO_TOKEN";
    }

}
