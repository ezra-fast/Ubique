#include <string>

std::string run_command(const char * command) {

    FILE * pipe = popen(command, "r");
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
