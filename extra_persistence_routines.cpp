// extra_persistence_routines() calls scheduled_job_persistence() and startup_script_persistence()
// extra_persistence_routines() is only called if established_persistence_count < 1, as a last ditch effort to make sure that persistence is established before looping starts

#include "crontab_persistence.h"
#include "get_shell_startup_script.h"
#include <iostream>

bool startup_script_persistence();

void extra_persistence_routines() {

    startup_script_persistence();

}

bool startup_script_persistence() {
    std::string shell_startup_file = get_shell_startup_script();
    std::cout << shell_startup_file << '\n';
}


