// scheduled_job_persistence.cpp
// this function establishes persistence on the target machine by installing a malicious systemd timer job as the current user

// IMPORTANT: without root privileges, it is impossible to have user-privileged systemd timers execute because user-privileged jobs only execute during a corresponding logon session
// root privileges are needed to enable 'lingering', which removes this logon session requirement

#include <iostream>

bool scheduled_job_persistence() {
    /*
     * systemctl list-timers --all
     *
     * https://documentation.suse.com/smart/systems-management/html/systemd-working-with-timers/index.html
     *
     * all user-privileged jobs go in: ~/.config/systemd/{user}/
     *
     * */
}


