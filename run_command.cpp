// run_command(command, stderr_redirection_location) runs a shell command provided as argument 1 and returns the STDOUT and STDERR streams of the process as an std::string

#include <string>

std::string run_command(const char * command, std::string STDERR_DIRECTION_FLAG) {

    std::string full_command = std::string(command) + " 2>" + STDERR_DIRECTION_FLAG;

    FILE * pipe = popen(full_command.c_str(), "r");
    if (!pipe) {return "Failed.";}

    char buffer[1024];

    std::string result = "";

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    pclose(pipe);
    return result;          // this will contain the output of the command; 
                            // if the command fails, the output will be "Failed."
}
