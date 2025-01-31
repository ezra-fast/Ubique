// get_shell_startup_script() returns the shell type (bash, ash, csh, zsh, etc.) of the current environment as an std::string

#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>

std::string get_shell_startup_script() {
	/*
	 * 1. grab the environment's shell	-> std::getenv("SHELL")
	 * 2. construct a string for the shell's startup file (.{shell_type}rc)
	 * 3. return the startup script file name as a string
	 * */
	
	const char * environment_call = std::getenv("SHELL");
    if (!environment_call) {
        return "FAILED_NO_SHELL";
    }

    std::string current_shell = environment_call;           // converting the const char * to an std::string

    std::stringstream split_stream(current_shell);
    std::string holder;

    char delimeter = '/';

    while (!split_stream.eof()) {                                   // while there are tokens, keep reading
        getline(split_stream, holder, delimeter);                   // get the current token (section of string before the delimeter)
    }                                                               // the contents of holder at the end of this loop are the shell (the end of /usr/bin/shell)

    std::stringstream startup_script_path;
    startup_script_path << '.' << holder << "rc";

    // std::cout << "[+] Length: " << startup_script_path.str().length() << '\n';
    return startup_script_path.str();
}

