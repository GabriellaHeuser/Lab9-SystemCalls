/*****************************************************************************
 * Filename: pmod.c
 * Author: Gabi Heuser
 * Assignment: Lab 9 - System Calls
 * Section: CPE2600 121
 * Executable: ./pmod
 * Purpose: Lowers own priority by 10, sleeps for 1,837,272,638 ns,
 *          then prints bye and exits.
 ****************************************************************************/
#include <stdio.h>
#include <unistd.h>   // nice
#include <errno.h>    // errno
#include <time.h>     // nanosleep, struct timespec

int main(int argc, char* argv[])
{
    //nice() adds argument to the process's current nice value
    //higher nice value = lower priority
    //using 10 reduces priority by 10
    //nice() returns the new nice value
    //-1 is a valid nice value, so need errno for error detection
    errno = 0;
    int niceVal = nice(10);
    if (niceVal == -1 && errno != 0){ //ensuring successful nice()
        perror("nice failed");
        return 1;
    }

    //nanosleep pauses process for a requested time
    // Takes a struct timespec
    // tv_nsec must be less than 1,000,000,000 (1 s)
    // Must split into 1 s and 837,272,638 ns
    struct timespec duration;
    duration.tv_sec = 1;
    duration.tv_nsec = 837272638;

    //nanosleep returns 0 on success, -1 on error
    // Second argument can receives remaining time which we aren't using, so NULL
    if(nanosleep(&duration, NULL)!=0){ //error checking
        perror("nanosleep failed");
        return 1;
    }

    //Exit Program
    printf("Exiting Program...\nGoodbye!\n");

    return 0;
}